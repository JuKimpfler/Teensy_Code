#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"

class DefenderTacticsC{
    private:
    public:
        void step();
        void BallCaught();
        void Ballsearch();
        void Goaldirection();
};
extern DefenderTacticsC DefenderTactics;