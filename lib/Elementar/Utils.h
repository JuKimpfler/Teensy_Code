#pragma once
#include "Arduino.h"

class UtilsC {

    private:

    public:
        bool Ran(float In,float min, float max);
        double Circel(double Angle);
        //int16_t Circel(int16_t Angle);
        //float distanceFromVectors(Vec2 vector, Vec2 otherVector);
};

extern UtilsC U;

