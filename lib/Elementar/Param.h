#pragma once
#include "Arduino.h"

// Parameter

#define HighSpeed 100 // Speed im HS modus
inline float LowSpeed = 70; // Speed im LS modus

inline float Kp =  0.09;
inline float Kd =  14;
inline float Ki =  0;

inline float PID_Konstante = 0.6; // 0.3

inline int LDR_Schwelle =  500;

inline int Line_Schwelle = 2000;

inline int Motor_Frequency =  400;

#define Interface_Frequency 10 // angegeben in Hz (wie oft pro sekunde wird das Interface upgedated)
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