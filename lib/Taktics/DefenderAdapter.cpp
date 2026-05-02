#include "DefenderAdapter.h"
#include "BNO055.h"

// Global adapter instance
DefenderAdapterC DefenderAdapter;

// ─────────────────────────────────────────────────────────────────────────────
// Static helpers
// ─────────────────────────────────────────────────────────────────────────────

float DefenderAdapterC::_norm360(float a) {
    a = fmod(a, 360.0f);
    if (a < 0.0f) a += 360.0f;
    return a;
}

// Ball.Angle is kept in −180..+180 (U.Circel convention).
// Defender expects 0..360.
float DefenderAdapterC::_toBallAngle360() {
    return _norm360(static_cast<float>(Ball.Angle));
}

// LineCalc.DriveAngle is in −180..+180.
// Defender checks for LinieWinkel near 0° (front) and near 180° (rear).
float DefenderAdapterC::_toLinieWinkel() {
    return _norm360(LineCalc.DriveAngle);
}

// Simple binary line-distance: 1.0 when any sensor is active, 0.0 otherwise.
// Defender uses this as a speed multiplier (100 * Line_distance), so 1.0 gives
// full speed on the line and 0.0 prevents movement when no line is detected.
float DefenderAdapterC::_getLineDistance() {
    return (Line.Summe > 0) ? 1.0f : 0.0f;
}

// Convert Line.line[32] (values 0/1) into a bool array.
// If DEFENDER_SENSOR_CW is set the index order is reversed so that the
// Defender's CCW angle maths (i * 11.25°) stays consistent.
void DefenderAdapterC::_buildLineSensors(bool out[32]) {
#if DEFENDER_SENSOR_CW
    for (int i = 0; i < 32; i++) {
        // Mirror: sensor at physical index i maps to angle (32 - i) % 32
        out[(32 - i) % 32] = (Line.line[i] != 0);
    }
#else
    for (int i = 0; i < 32; i++) {
        out[i] = (Line.line[i] != 0);
    }
#endif
}

// ─────────────────────────────────────────────────────────────────────────────
// CamData synthesis
// ─────────────────────────────────────────────────────────────────────────────

CamData DefenderAdapterC::_buildCamData() const {
    CamData d;

    // ── Goal angle ───────────────────────────────────────────────────────────
    // The Defender expects the *camera-relative* angle to the own goal as it
    // would appear from a back-facing camera.
    //
    // Because both goals are co-axial on the field, the camera-relative angle
    // to the opponent goal seen by the forward camera equals the angle the own
    // goal would make on a hypothetical back camera:
    //   • robot aligned   → Cam.give_Angle_Cam() ≈ 0  → own-goal cam ≈ 0 ✓
    //   • robot rotated R → Cam.give_Angle_Cam() ≈ R  → own-goal cam ≈ R ✓
    //
    // Fallback when camera is unavailable: use IMU yaw (TiltZ), which was
    // calibrated to 0 when the robot faces the opponent goal.
    if (useCam && Cam.isValid()) {
        d.GoalAgnle          = Cam.give_Angle_Cam();
        d.BlueGoal_visible   = true;   // any goal visible → treat as own goal
        d.YellowGoal_visible = false;
    } else {
        d.GoalAgnle          = static_cast<float>(BNO055.TiltZ);
        d.BlueGoal_visible   = false;
        d.YellowGoal_visible = false;
    }

    // ── Goal height (penalty-area proximity) ─────────────────────────────────
    // The original Defender uses GoalHeight > 60 to detect that the robot has
    // crossed the own-goal boundary and should not keep driving toward the goal.
    // Without a back camera we synthesise this from the _nearOwnGoal flag that
    // is maintained in update().
    d.GoalHeight = _nearOwnGoal ? 100 : 0;

    return d;
}

// ─────────────────────────────────────────────────────────────────────────────
// Main update
// ─────────────────────────────────────────────────────────────────────────────

void DefenderAdapterC::update() {
    // ── Inputs ───────────────────────────────────────────────────────────────
    bool   lineDetected = (Line.Summe > 0);
    float  linieWinkel  = _toLinieWinkel();
    float  lineDist     = _getLineDistance();
    float  ballAngle360 = _toBallAngle360();
    double ballDist     = static_cast<double>(Ball.Distance);

    // Cam_SW: tells the Defender whether a camera is fitted.
    // When true and no goal is visible, the Defender returns to the line.
    // When false, the Defender skips that check and relies on IMU angle only.
    bool camSW = useCam;

    // ── Penalty-area proximity tracking ──────────────────────────────────────
    // The goal-side boundary of the penalty area runs roughly perpendicular to
    // the field (horizontally in front of the own goal).  When the robot's line
    // sensors detect this boundary, LineCalc.DriveAngle is close to 0° (drive
    // forward = away from own goal).  We latch _nearOwnGoal and keep it set for
    // DEFENDER_NEAR_GOAL_HOLD_MS after the last detection so the robot has
    // enough time to exit the penalty area before the flag clears.
    if (lineDetected) {
        float da = LineCalc.DriveAngle;  // −180..+180
        bool onGoalSideLine = (da > -DEFENDER_GOAL_SIDE_TOLERANCE &&
                               da <  DEFENDER_GOAL_SIDE_TOLERANCE);
        if (onGoalSideLine) {
            _nearOwnGoal     = true;
            _nearOwnGoalTime = millis();
        } else {
            // Robot is on a different boundary; clear the flag immediately.
            _nearOwnGoal = false;
        }
    } else if (_nearOwnGoal &&
               (millis() - _nearOwnGoalTime > DEFENDER_NEAR_GOAL_HOLD_MS)) {
        _nearOwnGoal = false;
    }

    // ── Build Defender inputs ─────────────────────────────────────────────────
    bool    lineSensors[32];
    CamData camData = _buildCamData();

    _buildLineSensors(lineSensors);

    // Last_Line_Angle: Defender uses this as a reference after line is lost.
    // We maintain _lastLineAngle ourselves (updated when line is detected) and
    // also rely on Defender's internal static for the same purpose.
    float lastLineAngle = linieWinkel;  // updated each cycle when line active

    // ── Call Defender ─────────────────────────────────────────────────────────
    _drive = _defender.follow_Line(
        lineSensors,
        ballAngle360,
        camData,
        linieWinkel,
        lineDist,
        lineDetected,
        lastLineAngle,
        ballDist,
        camSW
    );

    // ── Apply drive output ────────────────────────────────────────────────────
    // Defender outputs direction in 0..360.
    // Robot.Drive() expects direction in −180..+180 (same convention as
    // Cam.give_Angle() / LineCalc.DriveAngle used elsewhere in main.cpp).
    float dir = _drive.direction;
    if (dir > 180.0f) dir -= 360.0f;

    Robot.Drive(dir, 0.0f, static_cast<int>(_drive.speed));

    // ── Kicker ───────────────────────────────────────────────────────────────
    // Trigger the physical kicker when the Defender activates its kick drive.
    if (_defender.isKicking()) {
        Robot.Kicker.On();
    }
}
