#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calc.h"

class GameC{
    private:
        int Rand_Dir;
        bool once = false;
    public:
        void Run();
        void Stop();
        float dead_zone;
        float dead_diff;
        bool outside = false;
        float temp3_Angle = 0; 
};

extern GameC Game;