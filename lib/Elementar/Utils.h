#pragma once
#include "Arduino.h"

class UtilsC {

    private:

    public:
        bool isinRange(float In,float min, float max);
        //float distanceFromVectors(Vec2 vector, Vec2 otherVector);
};

extern UtilsC Utils;

