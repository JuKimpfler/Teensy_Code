#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"

class DefenderTacticsC{
    private:
    public:
        void step();
        void GoalAttak();
        void BallSearch();
        void GoalTurn();
        void Homing();
};
extern DefenderTacticsC DefenderTactics;