#pragma once

// ─────────────────────────────────────────────────────────────────────────────
// DefenderAdapter
// ─────────────────────────────────────────────────────────────────────────────
//
// PURPOSE
//   Bridge between Defender.cpp (which assumes a back-facing camera that sees
//   the own goal) and the actual robot hardware (which has only a forward-
//   facing camera pointing at the opponent's goal).
//
// CAMERA MAPPING
//   The Defender uses CamData::GoalAgnle as the *camera-relative* angle to the
//   own goal as seen by a back camera.  Because both goals are co-axial along
//   the field, the camera-relative angle to the opponent goal (forward camera)
//   and the camera-relative angle to the own goal (back camera) have the same
//   magnitude and sign when the robot is rotating:
//
//     • Robot centred and facing opponent → both cameras see their respective
//       goal at 0°.
//     • Robot rotates N° right → forward camera sees opponent goal at +N°;
//       a back camera would also see own goal at +N°.
//
//   Therefore: GoalAgnle ← Cam.give_Angle_Cam()
//   Fallback (camera invalid): GoalAgnle ← BNO055.TiltZ (IMU yaw)
//
// PENALTY-AREA PROTECTION
//   The original Defender uses CamData::GoalHeight to detect that the robot
//   has crossed the own-goal boundary (GoalHeight > 60 = "close to own goal").
//   Without a back camera, the adapter tracks the last "goal-side" line-sensor
//   activation and synthesises an equivalent GoalHeight.
//
// SENSOR LAYOUT ASSUMPTION
//   Line.line[32] is indexed 0..31 representing 360°/32 = 11.25° increments,
//   where index 0 corresponds to the robot's FORWARD direction and indices
//   increase counter-clockwise (or clockwise – configure SENSOR_CW below).
//   Verify this against your physical build and adjust if necessary.
//
// USAGE
//   In loop():
//     if (System.Start) {
//         DefenderAdapter.update();   // drives Robot automatically
//     }
//
// ─────────────────────────────────────────────────────────────────────────────

#include "Defender.h"
#include "Lowlevel.h"
#include "Calc.h"

// ── Compile-time configuration ────────────────────────────────────────────────

// Set to true  if sensor indices increase CLOCKWISE  (when viewed from above).
// Set to false if sensor indices increase COUNTER-CLOCKWISE.
// This affects find_Closest_Sensor in Defender; verify with your hardware.
#ifndef DEFENDER_SENSOR_CW
  #define DEFENDER_SENSOR_CW false
#endif

// Drive-away angle tolerance (degrees) used to classify the active line as the
// "goal-side" boundary (penalty arc).  When DriveAngle is within this range of
// 0°, the line sensors are touching the boundary toward the own goal.
#ifndef DEFENDER_GOAL_SIDE_TOLERANCE
  #define DEFENDER_GOAL_SIDE_TOLERANCE 45.0f
#endif

// How long (ms) to keep the _nearOwnGoal flag set after the goal-side line
// was last detected.  Gives the robot time to exit the penalty area.
#ifndef DEFENDER_NEAR_GOAL_HOLD_MS
  #define DEFENDER_NEAR_GOAL_HOLD_MS 1200u
#endif

// ── Class ─────────────────────────────────────────────────────────────────────

class DefenderAdapterC {
public:
    // Call once per control loop.  Computes the Defender drive output and
    // applies it through Robot.Drive().  Also fires Robot.Kicker when the
    // Defender decides to kick.
    void update();

    // Enable / disable camera usage inside the Defender logic.
    // When false, the IMU yaw is used as goal-angle fallback and the Defender
    // will not perform the "camera-off → return to line" early-exit.
    bool useCam = true;

    // Read-only access to the last computed drive data (for telemetry).
    Drive_Data getDrive() const { return _drive; }

private:
    Defender   _defender;
    Drive_Data _drive;

    // Penalty-area proximity tracking
    bool     _nearOwnGoal     = false;
    uint32_t _nearOwnGoalTime = 0;   // millis() when flag was last set

    // ── Helpers ───────────────────────────────────────────────────────────────

    // Synthesise CamData from the forward camera + IMU.
    CamData _buildCamData() const;

    // Convert Line.line[32] (uint16_t 0/1) to a bool array.
    // Sensor order is reversed if DEFENDER_SENSOR_CW is true so that the
    // Defender's angle arithmetic always assumes CCW layout.
    static void _buildLineSensors(bool out[32]);

    // Convert Ball.Angle (-180..+180) → 0..360 as expected by Defender.
    static float _toBallAngle360();

    // Convert LineCalc.DriveAngle (-180..+180) → 0..360.
    static float _toLinieWinkel();

    // Normalised line-distance (0..1) fed into Defender.
    static float _getLineDistance();

    // Angle-normalisation helpers (0..360).
    static float _norm360(float a);
};

extern DefenderAdapterC DefenderAdapter;
