#pragma once
#include "Arduino.h"

#ifndef Glob
#define Glob
inline int Cycletime = 0;
inline int MainSpeed = 30;
inline int Cycle_P2 = 0;
inline int Cycle_P3 = 0;
inline double Mess_Time;
inline String Zone;
inline int LDR_Schwelle;
inline int IR_Dist_Offset;
inline uint16_t IR_maxi[16];
inline int16_t IR_mini[16];
#endif