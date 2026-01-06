#pragma once
#include <structs.h>
#include <Param.h>
#include <math.h>

class BallCalcC{
    public:
        float OutAngle;
        void getAngle(float inAngle, float dist);
};

extern BallCalcC BallCalc;