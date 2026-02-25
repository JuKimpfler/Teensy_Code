#pragma once
#include "Arduino.h"

#ifndef Glob
#define Glob
inline int Cycletime = 0;
inline int MainSpeed = 30;
inline int Cycle_P2 = 0;
inline int Cycle_P3 = 0;
inline bool Line_Follow = false; // Switch to follow line
inline float Front_Speed = 2; // Divisor to follow line
#endif