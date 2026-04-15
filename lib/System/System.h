#pragma once
#include "Elementar.h"
#include "Lowlevel.h"
#include "Calc.h"
#include "Game.h"


class SystemC {
    private:
                  
    public:
        void begin(int CIndex);

        bool Start=false;
        bool Button[4];
        bool Switches[4];

        class UpdateC{
            private:
            public:
                void Sensors();
                void Interface();
                void Calculations();
        };
        UpdateC Update;
};


extern elapsedMillis Interface_Timer;
extern elapsedMillis Schuss_Timer;
extern elapsedMicros Cycle_Timer;

extern SystemC System;
