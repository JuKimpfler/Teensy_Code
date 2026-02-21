#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"
#include "Elementar_Taktics.h"
#include "System.h"
#include "Attacker.h"
#include "LineCalculations.h"

class GameC{
    private:
        int Rand_Dir;
    public:
        void Run();
};

extern GameC Game;
 