#ifndef Defender_H
#define Defender_H

#include <math.h>
#include <Arduino.h>
#include "Cam.h"

struct Drive_Data
{
    float turn;
    float direction;
    float speed;
};
struct KickState {
  float    ball_angle   = 0.0f;
  double   ball_dist    = 0.0;
  uint32_t still_start  = 0;
  uint32_t kick_start   = 0;
  bool     kicking      = false;
};

class Defender
{
private:
  float speed = 30;
  bool first_out = true;
  int out_timer = 0;
  int Kick_Starter = 0; 
  bool Kick_Active = false;
  float change = 0;
  float ballmovementCheckAngle = 0;
  float ballmovementCheckDist = 0;
  int timer_last = 0;
  int timer = 0;
  int Closest_sesnor; //Closest sensor by angle to Ball angle difference
  CamData camData_Back;
  Drive_Data drive_data;
  const int defender_out_speed = 30;
  void return_to_Line(int out_timer, bool Goal_Detected, float GoalAngle);
  bool handle_Ball_In_Front(float ball_angle, float Goal_angle, float LinieWinkel, float Line_distance);
  void find_Closest_Sensor(bool Line_sensors_active[], float ball_angle, float ball_x_distance, float speed);
  bool check_Kick_Trigger(KickState &kick_state, float ball_angle, double ball_distance);
  Drive_Data handle_Kick(KickState &kick_state, float ball_angle);
  bool is_Ball_Still(KickState &kick_state, float ball_angle, double ball_distance);
  // Constants
  static const uint32_t STILL_DURATION_MS      = 2000;
  static const int KICK_DURATION_MS       = 500;
  static const int KICK_SPEED             = 70;


  static constexpr float KICK_DIST_MIN         = 5.0f;
  static constexpr float KICK_DIST_MAX         = 15.0f;
  static constexpr float MOVEMENT_DIST_THRESH  = 3.0f;
  static constexpr float MOVEMENT_ANGLE_THRESH = 15.0f;


public:
  Drive_Data follow_Line(bool Line_sensors_active[], float ball_angle, CamData camData_Back, float LinieWinkel, float Line_distance, bool Line_Detected, float Last_Line_Angle, double ball_distance, bool Cam_SW);
  double normalizeAngle(double angle);
  double AtoT(double angleA);
  double TtoA(double angleB);
};

#endif