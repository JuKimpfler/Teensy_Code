#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"
#include "Elementar_Taktics.h"
#include "Attacker.h"
#include "Defender.h"

class GameC{
    private:
        int Rand_Dir;
        int myAttackChances;
        int oppAttackChances;
        int subOppAttackChances;
        bool AttackerPreference = true;
    public:
        void Run();
};

extern GameC Game;
 