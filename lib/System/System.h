#pragma once
#include "Elementar.h"
#include "BNO055.h"
#include "Expander.h"
#include "Motor.h"
#include "PidCalculations.h"
#include "Line.h"
#include "Taktics.h"
#include "Calculations.h"
#include "Cam.h"
#include "LDR.h"
#include "IR.h"
#include "Robot.h"
#include "RGB.h"
#include "Cam.h"
#include "Mouse.h"
#include "US.h"


class SystemC {
    private:
        void Start_Update();
                    
    public:

        bool Start;
        bool Button[4];
        bool Switches[4];
        void init();
        void Button_Update();

        void Calibrate(String NR);

        class UpdateC{
            private:
                void printPID();
                int inputAngle=0;
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
