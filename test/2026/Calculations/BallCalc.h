#pragma once
#include <structs.h>
#include <Param.h>
#include <math.h>
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