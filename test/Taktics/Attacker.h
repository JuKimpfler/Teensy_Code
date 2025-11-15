#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"

class AttackerTacticsC{
    private:
    public:
        void step();
        void BallCaught();
        void BallSearch();
};

extern AttackerTacticsC AttackerTactics;