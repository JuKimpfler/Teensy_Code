#pragma once
#include "Arduino.h"

// Modus wahl
#define Robo_w // Robo_w oder Robo_s für Roboter Auswahl

#define Debug_EN // Debug Ausgabe
//#define Ir_Calib // calibration Mode IR Values
//#define Calib // calibration Mode Normal

#define HighSpeed 45 // Speed im HS modus
inline float LowSpeed = 30; // Speed im LS modus

inline float Kp = 3; // Startwert, dann erhöhen
inline float Kd = 17.0; // Startwert, dann erhöhen
inline float Ki = 0; // Startwert, dann erhöhen
inline float PID_Mult = 0.1; // Belassen

inline float PID_Konstante = 0.5; // 0.3

inline double E_Faktor = 1.2;
inline double B_Faktor = 1.2;
inline double F_Faktor = 1.55;
inline double C_Faktor = 2;
inline double H_Faktor = 1.5;

inline int Dist_Schwelle_min = 10; // Zonen Schwelle Dist 
inline int Dist_Schwelle_max = 60; // Zonen Schwelle Dist 

inline int LDR_Schwelle_w = 500;
inline int LDR_Schwelle_s = 500; 

inline int IR_Dist_Offset_w = 79;
inline int IR_Dist_Offset_s = 100;

const int16_t IR_mini_s[16] = {16,24,15,17,19,17,16,18,14,17,14,15,54,17,16,0};
const uint16_t IR_maxi_s[16] = {2920,2920,2854,2968,2958,3016,2985,2968,2664,2713,2935,2279,2914,2873,2850,56};
const int16_t IR_mini_w[16] = {29,34,29,31,28,30,30,27,26,25,12,19,145,28,29,0};
const uint16_t IR_maxi_w[16] = {2127,2056,2021,1849,2157,2187,2100,2100,1864,1832,379,1643,2222,2071,2065,40};

inline int Line_Grass = 2000;
inline int Line_Norm = 4000;
inline int Line_Grass_VW = 2000;
inline int Line_Norm_VW = 4000;

inline int Line_Schwelle_VW = 3200;
inline int Line_Schwelle = 3200;

inline int IR_Sight = 30; // ab wann wird der Ball als da erkannt

inline int Motor_Frequency =  400;

#define Interface_Frequency 20 // angegeben in Hz (wie oft pro sekunde wird das Interface upgedated)
#define US_Frequency 10 // angegeben in Hz (wie oft pro sekunde wird das US upgedated)