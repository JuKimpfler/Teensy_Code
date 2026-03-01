#pragma once
#include "Arduino.h"

// Parameter

#define HighSpeed 45 // Speed im HS modus
inline float LowSpeed = 30; // Speed im LS modus

inline float Kp = 3; // Startwert, dann erhöhen
inline float Kd = 17.0; // Startwert, dann erhöhen
inline float Ki = 0; // Startwert, dann erhöhen
inline float PID_Mult = 0.1; // Belassen

inline float PID_Konstante = 0.5; // 0.3

inline double E_Faktor = 1.4;
inline double B_Faktor = 1.14;
inline double F_Faktor = 1.45;
inline double C_Faktor = 1.5;

inline int LDR_Schwelle =  500;
inline int LDR_Schwelle_s = 500; 

inline int Line_Schwelle = 3200;

inline int Motor_Frequency =  400;

#define Interface_Frequency 20 // angegeben in Hz (wie oft pro sekunde wird das Interface upgedated)
#define US_Frequency 10 // angegeben in Hz (wie oft pro sekunde wird das US upgedated)

// CalculationsParameter
#define FocalLength 2040
#define ImageWidth 2952
#define ImageHeight 1944
#define ObjectSize 4.267

#define distFactor 2.3

inline int IR_Pos[8] = {1,2,3,4,5,6,7,8};
inline int Ir_Min[8] = {1,2,3,4,5,6,7,8};
inline int Ir_Max[8] = {1,2,3,4,5,6,7,8};

//#define XCP_BL 
//#define XCP_USB