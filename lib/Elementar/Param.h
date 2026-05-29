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
inline int16_t IR_mini_w_calib[16] = {19,20,18,18,21,18,18,18,18,21,19,19,0,19,17,17};
inline uint16_t IR_maxi_w_calib[16] = {2941,2917,2906,2917,2940,2922,2932,2922,2835,2941,2941,2922,488,2924,2931,2916};

inline int Line_w_calib[40] = {2716,2758,2749,2638,2656,2670,2633,2759,2705,2866,2889,2877,2759,2914,2879,2718,2778,2912,2767,2763,2744,2754,2782,2788,2748,2738,2774,2803,2748,2757,2587,2789,2638,2893,2829,2945,3026,2744,2731,2923};
inline int Line_s_calib[40] = {2716,2758,2749,2638,2656,2670,2633,2759,2705,2866,2889,2877,2759,2914,2879,2718,2778,2912,2767,2763,2744,2754,2782,2788,2748,2738,2774,2803,2748,2757,2587,2789,2638,2893,2829,2945,3026,2744,2731,2923};
inline int Line_calib_min[40] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
inline int Line_calib_max[40] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

inline int LDR_s_Calib = 100;
inline int LDR_w_Calib = 100;

inline int Drib_w = 1210;
inline int Drib_s = 1340;
inline int Drib_schwelle = 0;

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

inline bool preLineAvoidance = false;
inline bool preLineDec = true;

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