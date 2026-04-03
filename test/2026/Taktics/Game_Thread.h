#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calculations.h"
#include "Elementar_Taktics.h"
#include "System.h"
#include "Attacker.h"

class GameC{
    private:
        int Rand_Dir;
        bool once = false;
    public:
        void Run();
        float dead_zone;
        float dead_diff;
        bool outside = false;
        float temp3_Angle = 0; 
};

extern GameC Game;
 