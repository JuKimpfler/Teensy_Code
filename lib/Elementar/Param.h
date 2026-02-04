#pragma once
#include "Arduino.h"

// Parameter

#define HighSpeed 30 // Speed im HS modus
inline double LowSpeed = 10; // Speed im LS modus

inline double Kp =  2.2; //2.2 
inline double Kd =  12; //10
inline double Ki =  0.0016; //.00016

inline double Drive_Smoothed_Faktor = 0.5;

inline double PID_Mult = 0.1;

inline double PID_Konstante = 0.6; // 0.3

inline int LDR_Schwelle =  500;

inline int Line_Schwelle = 2900;

inline int Motor_Frequency =  400;

#define Interface_Frequency 10 // angegeben in Hz (wie oft pro sekunde wird das Interface upgedated)
#define US_Frequency 10 // angegeben in Hz (wie oft pro sekunde wird das US upgedated)

// CalculationsParameter
#define FocalLength 2040
#define ImageWidth 2952
#define ImageHeight 1944
#define ObjectSize 4.267

#define distFactor 2.3

#define TURN_TO_GOAL true

inline int IR_Pos[8] = {1,2,3,4,5,6,7,8};
inline int Ir_Min[8] = {1,2,3,4,5,6,7,8};
inline int Ir_Max[8] = {1,2,3,4,5,6,7,8};

//#define XCP_BL 
//#define XCP_USB