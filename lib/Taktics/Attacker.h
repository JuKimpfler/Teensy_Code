#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"
#include "Elementar_Taktics.h"

class AttackerTacticsC{
    private:
    public:
        void step();
        void GoalAttak();
        void BallSearch();
        void EckEscape();
};

extern AttackerTacticsC AttackerTactics;