#pragma once
#include "Arduino.h"

// Modus wahl
#define Robo_s // Robo_w oder Robo_s für Roboter Auswahl

#define Debug_EN // Debug Ausgabe
//#define Ir_Calib // calibration Mode IR Values
//#define Calib // calibration Mode Normal
//#define Line_Calib

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

inline int IR_Dist_Offset_w = 90;
inline int IR_Dist_Offset_s = 124;

const int16_t IR_mini_s[16] = {23,24,26,25,19,23,25,19,25,22,1,19,147,22,24,14};
const uint16_t IR_maxi_s[16] = {2950,2950,2950,2950,2950,2950,2950,2950,2950,2950,535,2939,3018,2968,2955,2938};
const int16_t IR_mini_w[16] = {23,24,26,25,19,23,25,19,25,22,1,19,147,22,24,14};
const uint16_t IR_maxi_w[16] = {2950,2950,2950,2950,2950,2950,2950,2950,2950,2950,535,2939,3018,2968,2955,2938};

inline int Line_Grass = 2000;
inline int Line_Norm = 4000;
inline int Line_Grass_VW = 2100;
inline int Line_Norm_VW = 3800;

inline int Line_Schwelle_VW = 3100;
inline int Line_Schwelle = 3200;

inline int IR_Sight = 45; // ab wann wird der Ball als da erkannt

inline int Motor_Frequency =  400;

#define Interface_Frequency 20 // angegeben in Hz (wie oft pro sekunde wird das Interface upgedated)
#define US_Frequency 10 // angegeben in Hz (wie oft pro sekunde wird das US upgedated)