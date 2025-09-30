#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"

class DefenderTacticsC{
    public:
        void step();
        void BallSearch();
        void BallDefend();
};

extern DefenderTacticsC DefenderTactics;