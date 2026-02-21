#pragma once
#include <structs.h>
#include <Param.h>
#include <math.h>

class BallCalcC{
    public:
        double DriveAngle;
        void getAngle(double inAngle, double dist);
        void calcTorAngle();
        void CalcAngle();
        void CalcDist();
};

extern BallCalcC BallCalc;