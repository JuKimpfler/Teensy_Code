#pragma once
#include "Arduino.h"

#define HighSpeed 30 // Speed im HS modus
#define LowSpeed = 25; // Speed im LS modus

inline float Kp = 1.1; // Startwert, dann erhöhen:  1
inline float Kd = 12.0; // Startwert, dann erhöhen:  1
inline float Ki = 0; // Startwert, dann erhöhen
inline float PID_Mult = 0.1; // Belassen
inline float PID_Konstante = 0.5; // Belassen

inline int16_t IR_mini_s_calib[16] = {19,20,18,18,21,18,18,18,18,21,19,19,0,19,17,17};
inline uint16_t IR_maxi_s_calib[16] = {2941,2917,2906,2917,2940,2922,2932,2922,2835,2941,2941,2922,488,2924,2931,2916};
inline int16_t IR_mini_w_calib[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
inline uint16_t IR_maxi_w_calib[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

inline int Line_w_calib[40] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
inline int Line_s_calib[40] = {2691,2477,2618,2575,2384,2833,2780,3030,2714,2811,3158,2738,2816,2836,2540,2380,2424,2432,2301,2465,2417,2835,2644,1988,2191,2390,1970,2541,2574,2643,2703,2634,2820,2843,2550,2474,1929,2201,2605,2994};
inline int Line_calib_min[40] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
inline int Line_calib_max[40] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

inline int LDR_s_Calib = 100;
inline int LDR_w_Calib = 100;

inline int16_t IR_mini_conf[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
inline uint16_t IR_maxi_conf[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
inline int Line_conf[40] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
inline int LDR_conf = 100;

inline int* Line_Schwelle_calib[] = {Line_calib_min,Line_calib_max};
inline int LDR_Schwelle[] = {LDR_s_Calib,LDR_w_Calib};
inline int* Line_Schwelle[] = {Line_s_calib,Line_w_calib};
inline int16_t* IR_min[] = {IR_mini_s_calib,IR_mini_w_calib};
inline uint16_t* IR_max[] = {IR_maxi_s_calib,IR_maxi_w_calib};

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