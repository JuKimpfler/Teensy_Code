#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"
#include "Elementar_Taktics.h"

class AttackerTacticsC{
    private:
    public:
        void step();
        void BallCaught();
        void Ballsearch();
};

extern AttackerTacticsC AttackerTactics;