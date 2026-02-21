#pragma once
#include "Arduino.h"

class UtilsC {

    private:

    public:
        bool isinRange(double In,double min, double max);
        //double distanceFromVectors(Vec2 vector, Vec2 otherVector);
};

extern UtilsC Utils;

