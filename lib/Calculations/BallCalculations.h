#pragma once
#include <structs.h>
#include <Param.h>
#include <math.h>

class BallCalcC{
    public:
        float DriveAngle;
        void getAngle(float inAngle, float dist);
        void CalcAngle();
        void CalcDist();
};

extern BallCalcC BallCalc;