#pragma once
#include "Arduino.h"


//#define Debug_EN // Debug Ausgabe
#define Ir_Calib // calibration Mode IR Values
//#define Calib // calibration Mode Normal
//#define Line_Calib
//#define PID_Calib
//#define Com_Debug 

#define HighSpeed 30 // Speed im HS modus
#define LowSpeed = 25; // Speed im LS modus

inline float Kp = 1.1; // Startwert, dann erhöhen:  1
inline float Kd = 12.0; // Startwert, dann erhöhen:  1
inline float Ki = 0; // Startwert, dann erhöhen
inline float PID_Mult = 0.1; // Belassen
inline float PID_Konstante = 0.5; // Belassen

inline int LDR_Schwelle_w = 500;
inline int LDR_Schwelle_s = 600; 

inline int IR_Dist_Offset_w = 90;
inline int IR_Dist_Offset_s = 124;

const int16_t IR_mini_s[16] = {19,20,18,18,21,18,18,18,18,21,19,19,0,19,17,17};
const uint16_t IR_maxi_s[16] = {2941,2917,2906,2917,2940,2922,2932,2922,2835,2941,2941,2922,488,2924,2931,2916};
const int16_t IR_mini_w[16] = {23,24,26,25,19,23,25,19,25,22,1,19,147,22,24,14};
const uint16_t IR_maxi_w[16] = {2950,2950,2950,2950,2950,2950,2950,2950,2950,2950,535,2939,3018,2968,2955,2938};

// Calibration auf braunem ordner und weisem blatt papier ( Line mit nur weißer LED )
const uint16_t Line_max_w[32] = {3868, 3852, 3864, 3864, 3852, 3868, 3864, 3876, 3872, 3872, 3884, 3876, 3880, 3884, 3880, 3880, 3880, 3876, 3864, 3864, 3856, 3876, 3872, 3832, 3852, 3860, 3812, 3856, 3864, 3868, 3868, 3868};
const uint16_t Line_min_w[32] = {1104, 932, 1024, 1084, 904, 1264, 1168, 1656, 1424, 1436, 1688, 1280, 1356, 1528, 1504, 1604, 1540, 1492, 1060, 1060, 900, 1244, 1328, 740, 936, 1116, 628, 976, 1052, 1132, 1288, 1128};

// Calibration auf braunem ordner und weisem blatt papier ( Line mit roter LED )
const uint16_t Line_max_r[32] = {3868, 3852, 3864, 3864, 3852, 3868, 3864, 3876, 3872, 3872, 3884, 3876, 3880, 3884, 3880, 3880, 3880, 3876, 3864, 3864, 3856, 3876, 3872, 3832, 3852, 3860, 3812, 3856, 3864, 3868, 3868, 3868};
const uint16_t Line_min_r[32] = {1104, 932, 1024, 1084, 904, 1264, 1168, 1656, 1424, 1436, 1688, 1280, 1356, 1528, 1504, 1604, 1540, 1492, 1060, 1060, 900, 1244, 1328, 740, 936, 1116, 628, 976, 1052, 1132, 1288, 1128};

inline int Line_Schwelle = -400;

inline int LDR_Schwelle = 100;

inline int IR_Sight = 45; // ab wann wird der Ball als da erkannt

inline int Motor_Frequency =  400;

// --- Kamera Konstanten ---
const float CAM_RES_X = 320.0;
const float CAM_CENTER_X = CAM_RES_X / 2.0;
const float FOCAL_LENGTH_PX = 266.6;  // MUSS KALIBRIERT WERDEN!
const float GOAL_WIDTH_CM = 60.0;
const float BLUE_GOAL_X = 91.0;
const float BLUE_GOAL_Y = 243.0;
const float YELLOW_GOAL_X = 91.0;
const float YELLOW_GOAL_Y = 0.0;

#define Interface_Frequency 20 // angegeben in Hz (wie oft pro sekunde wird das Interface upgedated)
#define US_Frequency 40 // angegeben in Hz (wie oft pro sekunde wird das US upgedated)