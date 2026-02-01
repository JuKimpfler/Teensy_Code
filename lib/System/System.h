#pragma once
#include "Elementar.h"
#include "BNO055.h"
#include "Expander.h"
#include "Motor.h"
#include "PidCalculations.h"
#include "Line.h"
#include "Calculations.h"
#include "Taktics.h"
#include "Cam.h"
#include "INA.h"
#include "LDR.h"
#include "IR.h"
#include "Robot.h"
#include "RGB.h"
#include "Cam.h"
#include "Mouse.h"
#include "BL.h"
//#include "UltraSonic.h"


class SystemC {
    private:
        void Start_Update();
        void Button_Update();
        int last_MF=0;

    public:
        bool Start;
        bool Button[4];
        bool Switches[4];
        bool Sys_LED;

        class initC{
            public:
                void Sensors();
                void Interface();
                void Motors();
        };
        initC init;
        class UpdateC{
            public:
                void Sensors();
                void Interface();
                void Calculations();
        };
        UpdateC Update;
};


extern elapsedMillis Interface_Timer;
extern elapsedMillis US_Timer;
extern elapsedMillis Schuss_Timer;
extern elapsedMicros Cycle_Timer;

extern SystemC System;
