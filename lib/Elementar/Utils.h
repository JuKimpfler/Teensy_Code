#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

class UtilsC {
public:
    bool Ran(float In, float min, float max);
    double Circel(double Angle);
    double CircelA(double Angle);
};

extern UtilsC U;

#endif

