#pragma once
#include "IR.h"

class BallCalcC{
    public:
        double DriveAngle;
        void getAngle();
        void calcTorAngle();
        void CalcAngle();
        void CalcDist();
};

extern BallCalcC BallCalc;