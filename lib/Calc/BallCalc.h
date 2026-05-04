#pragma once
#include "IR.h"

class BallCalcC{
    public:
        int Position1;
        int Position2;
        float lastA;
        float lastD;
        float diffA;
        float diffD;
        double DriveAngle;
        void getAngle();
        void calcTorAngle();
        void CalcAngle();
        void CalcDist();
};

extern BallCalcC BallCalc;
extern elapsedMillis Stilltimer;