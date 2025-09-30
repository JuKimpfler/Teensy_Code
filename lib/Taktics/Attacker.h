#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"

class AttackerTacticsC{
    private:
    public:
        void BallCaught();
        void BallSearch();
        void step();
};

extern AttackerTacticsC AttackerTactics;