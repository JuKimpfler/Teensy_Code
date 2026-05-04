#include "Defender.h"

// ─────────────────────────────────────────────────────────────────────────────
// Kick state machine
// ─────────────────────────────────────────────────────────────────────────────

bool Defender::is_Ball_Still(KickState& state, float ball_angle, double ball_distance) {
    float dist_change  = fabs(ball_distance - state.ball_dist);
    float angle_change = fabs(ball_angle    - state.ball_angle);
    if(angle_change > 180.0f) angle_change = 360.0f - angle_change;

    bool ball_moved = (dist_change  > MOVEMENT_DIST_THRESH) ||
                      (angle_change > MOVEMENT_ANGLE_THRESH);

    if(ball_moved) {
        state.ball_angle  = ball_angle;
        state.ball_dist   = ball_distance;
        state.still_start = millis();
        return false;
    }
    return (millis() - state.still_start >= STILL_DURATION_MS);
}

Drive_Data Defender::handle_Kick(KickState& state, float ball_angle) {
    if(millis() - state.kick_start < KICK_DURATION_MS) {
        drive_data.speed     = 100;
        drive_data.direction = ball_angle;
        return drive_data;
    }
    state.kicking    = false;
    state.ball_angle = ball_angle;
    state.still_start = millis();
    return drive_data;
}

bool Defender::check_Kick_Trigger(KickState& state, float ball_angle, double ball_distance) {
    bool in_range = (ball_distance >= KICK_DIST_MIN && ball_distance <= KICK_DIST_MAX);

    if(!in_range) {
        state.ball_angle  = ball_angle;
        state.ball_dist   = ball_distance;
        state.still_start = millis();
        return false;
    }

    if(is_Ball_Still(state, ball_angle, ball_distance)) {
        state.kicking    = true;
        state.kick_start = millis();
        return true;
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Line following helpers
// ─────────────────────────────────────────────────────────────────────────────

bool Defender::handle_Ball_In_Front(float ball_angle, float Goal_angle, float LinieWinkel, float Line_distance) {
    if((ball_angle - Goal_angle) <= 175 || (ball_angle - Goal_angle) >= 185)
        return false;

    if(Line_distance > 0.5f &&
       ((LinieWinkel > 340 && LinieWinkel < 20) ||
        (LinieWinkel > 160 && LinieWinkel < 200))) {

        drive_data.speed     = 100 * Line_distance;
        drive_data.direction = LinieWinkel;
    }
    return true;
}

void Defender::find_Closest_Sensor(bool Line_sensors_active[], float ball_angle, float ball_x_distance, float speed) {
    float min_angle_diff = 360.0f;

    for(int i = 0; i < 32; i++) {
        if(!Line_sensors_active[i]) continue;

        drive_data.speed   = fmin(speed, speed * exp(ball_x_distance / 50.0f - 1) - 0.31f);
        int angle_diff     = fabs((i * 11.25f) - ball_angle);
        if(angle_diff > 180) angle_diff = 360 - angle_diff;

        if(fabs(angle_diff) < min_angle_diff) {
            Closest_sesnor = i;
            min_angle_diff = angle_diff;
        }
    }
    drive_data.direction = Closest_sesnor * 11.25f;
}

void Defender::return_to_Line(int out_timer, bool Goal_Detected, float GoalAngle) {
    if(Goal_Detected) {
        GoalAngle += 180;
        if(GoalAngle < 0) GoalAngle += 360.0f;
        if(GoalAngle > 360.0f) GoalAngle -= 360.0f;
        drive_data.speed     = 30;
        drive_data.direction = GoalAngle;
    }
    else {
        drive_data.speed     = defender_out_speed;
        drive_data.direction = 0.0f;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Helper for angle normalization
// ─────────────────────────────────────────────────────────────────────────────

double Defender::normalizeAngle(double angle) {
    angle = fmod(angle, 360.0);
    if(angle < 0) angle += 360.0;
    return angle;
}

double Defender::AtoT(double angleA) {
    return normalizeAngle(90.0 - angleA);
}

double Defender::TtoA(double angleB) {
    return normalizeAngle(90.0 - angleB);
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN FUNCTION
// ─────────────────────────────────────────────────────────────────────────────

Drive_Data Defender::follow_Line(bool Line_sensors_active[], float ball_angle,
                                 CamData camData_Back, float LinieWinkel, float Line_distance,
                                 bool Line_Detected, float Last_Line_Angle, double ball_distance, bool Cam_SW) {

  float Goal_angle = static_cast<float>(camData_Back.GoalAgnle);
  bool Goal_Detected = camData_Back.BlueGoal_visible || camData_Back.YellowGoal_visible;
  timer = millis();

  // ─────────────────────────────────────────────────────────────────────────
  // Save last valid LineAngle
  // ─────────────────────────────────────────────────────────────────────────
  static float last_valid_line_angle = 0;
  static bool last_line_detected = false;

  if(Line_Detected) {
      last_valid_line_angle = LinieWinkel;
  }

  last_line_detected = Line_Detected;

  // ─────────────────────────────────────────────────────────────────────────
  // Ball X distance
  // ─────────────────────────────────────────────────────────────────────────
  float ball_x_distance = fabs(sin(fabs(ball_angle) * M_PI / 180.0f) * ball_distance);
  int ball_angle_before = ball_angle;

  ball_angle = fmod(ball_angle - Goal_angle + 360.0f, 360.0f);

  // ─────────────────────────────────────────────────────────────────────────
  // Kick logic
  // ─────────────────────────────────────────────────────────────────────────
  if(Kick_Active && timer - Kick_Starter < KICK_DURATION_MS) {
      Serial.println("Kick active");
      drive_data.speed     = KICK_SPEED;
      drive_data.direction = ball_angle_before;
  }
  else {
    Kick_Active = false;
    if(fabs(Goal_angle) > 47 || (Goal_Detected == false && Cam_SW == true)) {
      drive_data.speed     = 30;
      drive_data.direction = 0.0f;
      return drive_data;
    }
    // ─────────────────────────────────────────────────────────────────────
    // LINE LOST LOGIC
    // ─────────────────────────────────────────────────────────────────────
    if(!Line_Detected) {

      float diff = fabs(normalizeAngle(last_valid_line_angle - Goal_angle));
      bool robot_left_towards_goal = (diff < 90 || diff > 270);

      if(robot_left_towards_goal && camData_Back.GoalHeight > 60) {
        // Roboter ist Richtung Tor von der Linie runtergefahren → NICHT Richtung Tor fahren!
        drive_data.speed = defender_out_speed;
        drive_data.direction = last_valid_line_angle;
      } 
      else {
        // Roboter ist vom Tor weg runtergefahren → Richtung Tor fahren ist korrekt
        return_to_Line(out_timer, Goal_Detected, Goal_angle);
      }

      return drive_data;
    }

    // ─────────────────────────────────────────────────────────────────────
    // Normal line following
    // ─────────────────────────────────────────────────────────────────────
    if(Line_Detected) {
      first_out = true;

      if(!handle_Ball_In_Front(ball_angle, Goal_angle, LinieWinkel, Line_distance)) {
        find_Closest_Sensor(Line_sensors_active, ball_angle, ball_x_distance, speed);
        if(Line_distance > 0.7f && ((LinieWinkel > 340 && LinieWinkel < 20) || (LinieWinkel > 160 && LinieWinkel < 200))) {
          drive_data.speed     = 100 * Line_distance;
          drive_data.direction = LinieWinkel;
        }
      }
    }
    if(timer - timer_last > 2000) {
      Serial.print("Timer-----------");
      
      // Debug: Check each condition individually
      bool angle_change_ok = fabs(ball_angle_before - ballmovementCheckAngle) < 20.0f;
      bool dist_change_ok = fabs(ball_distance - ballmovementCheckDist) < 20.0f;
      bool ball_dist_min_ok = ball_distance > 30;
      bool ball_dist_max_ok = ball_distance < 55.0f;
      float ball_angle_check = fabs(((ball_angle_before+180)%360)-180);
      bool ball_angle_min_ok = ball_angle_check > 15.0f;
      bool ball_angle_max_ok = ball_angle_check < 40.0f;
      bool line_detected_ok = Line_Detected;
      
      Serial.print(" DEBUG: angle_change_ok=");
      Serial.print(angle_change_ok);
      Serial.print(" dist_change_ok=");
      Serial.print(dist_change_ok);
      Serial.print(" ball_dist_ok=");
      Serial.print(ball_dist_min_ok && ball_dist_max_ok);
      Serial.print(" ball_angle_ok=");
      Serial.print(ball_angle_min_ok && ball_angle_max_ok);
      Serial.print(" line_ok=");
      Serial.print(line_detected_ok);
      Serial.print(" | ");
      
      if(angle_change_ok && dist_change_ok && ball_dist_min_ok && ball_dist_max_ok && ball_angle_min_ok && ball_angle_max_ok && line_detected_ok)
      {
        Kick_Active = true;
        Kick_Starter = timer;
        Serial.println("KICK TRIGGERED!");
      }
      else{
        Serial.println("Conditions not met");
        if(!angle_change_ok) {
          Serial.print(" | Kick not executed because of angle change: ");
          Serial.println(String(fabs(ball_angle_before - ballmovementCheckAngle)));
        }
        if(!dist_change_ok) {
          Serial.print(" | Kick not executed because of distance change: ");
          Serial.println(String(fabs(ball_distance - ballmovementCheckDist)));
        }
        if(!ball_dist_min_ok || !ball_dist_max_ok) {
          Serial.print(" | Kick not executed because of ball distance: ");
          Serial.println(String(ball_distance));
        }
        if(!ball_angle_min_ok || !ball_angle_max_ok) {
          Serial.print(" | Kick not executed because of ball angle: ");
          Serial.println(String(ball_angle_check));
        }
        if(!line_detected_ok) {
          Serial.println(" | Kick not executed because of no line detected");
        }
      }
      ballmovementCheckAngle = ball_angle_before;
      ballmovementCheckDist  = ball_distance;
      timer_last = timer;
    } 
  }
  Serial.print("Ball angle bevor: ");
  Serial.print(fabs(((ball_angle_before+180)%360)-180));
  Serial.print(" AngleChange: " + String(fabs(ball_angle_before - ballmovementCheckAngle)));
  Serial.print(" DistChange: " + String(fabs(ball_distance - ballmovementCheckDist)));
  Serial.print(" | Kick active: " + String(Kick_Active));
  Serial.print(" | BallDist: " + String(ball_distance));
  Serial.print(" | Change: " + String(change));
  Serial.print(" | Drive direction: ");
  Serial.print(drive_data.direction);
  Serial.print(" | GoalAngle: ");
  Serial.print(Goal_angle);
  Serial.print(" | Ball angle: ");
  Serial.println(ball_angle);
  return drive_data;
}
