#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"

class Elem_TakticsC{
    private:
    public:
        void Ballsearch();
        void BallCaught();
        void CornerRightEscape();
        void CornerLeftEscape();
        void step();
};

extern Elem_TakticsC Elem_Taktics;
 