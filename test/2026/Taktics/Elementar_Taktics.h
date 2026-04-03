#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"
#include "IR.h"
#include "US.h"

class TakticsC{
    private:
    public:
        void BallSearch();
        void GoalAttak();
        void CornerRightEscape();
        void CornerLeftEscape();
        void step();
};

extern TakticsC Taktics;
 