#include "Elementar.h"
#include "Line.h"
#pragma once

class LineCalcC{
    private:
        double summex = 0;
        double summey = 0;
        double lastWinkel = 0;
    public:
        double RawAngle;
        int Distance;
        double DriveAngle;
        void Calc();
};

extern LineCalcC LineCalc;