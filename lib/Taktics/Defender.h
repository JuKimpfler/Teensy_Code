#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"
#include "Elementar_Taktics.h"

class DefenderTacticsC{
    private:
    public:
        void step();
        void BallCaught();
        void Defens();
};
extern DefenderTacticsC DefenderTactics;