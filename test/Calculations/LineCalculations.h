#include "Elementar.h"
#include "Line.h"
#pragma once

class LineCalcC{
    private:
        float summex = 0;
        float summey = 0;
    public:
        float RawAngle;
        int Distance;
        float DriveAngle;
        void Calc();
};

extern LineCalcC LineCalc;