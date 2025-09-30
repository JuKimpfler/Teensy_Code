#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"

class TakticsC{
    private:
    public:
        float AttackChances;

        void BallSearch();
        void GoalAttak();
        void CornerRightEscape();
        void CornerLeftEscape();
        void CalculateAttackChances();
};

extern TakticsC Taktics;
 