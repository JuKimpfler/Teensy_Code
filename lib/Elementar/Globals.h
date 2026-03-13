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
inline bool Goal_Turn;

inline bool ready1 = false;
inline bool ready2 = false;
inline bool ready3 = false;
inline bool ready4 = false;
inline bool ready5 = false;
#endif