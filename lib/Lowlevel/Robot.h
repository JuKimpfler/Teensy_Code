#pragma once
#include "Elementar.h"
#include "Motor.h"
#include "Expander.h"
#include "PidCalculations.h"
#include "IR.h"
#include "Cam.h"

extern elapsedMicros Drive_Smoothed_Timer;
extern elapsedMillis Kicker_Timer; // timer for kicker operations

class RobotC {
public:

    bool Start;
    bool Button[4];
    bool Switches[4];
    bool Sys_LED;

    int Drive_Smoothed_aktive = 0;
    double Drive_Smoothed_start = 0;
    double Drive_Smoothed_optimal = 0;
    double Drive_Smoothed_Dir = 0;
    double Drive_Smoothed_Angle = 0; 
    double Drive_Smoothed_last = 0;
    double Drive_Smoothed_diff = 0; 
    double Drive_Smoothed_diffnow = 0;

    void Turn(double Angel);

    void Drive_Smoothed_set(double Dir , double Angle , int sollSpeed , int istSpeed);
    void Drive_Smoothed_Update();
    void Drive_Smoothed_Kill();

    void Drive(double Dir , double Angel = 200000 ,int Speed1 = HighSpeed);
    void Stop();
    void Break();
    
    class KickerC{
        public:
            void Once(); // single pulse
            void On(int Cycletime); // start pulse cycling
            void Off(); // stop cycling
            void Update(); // internal timer handling
        private:
            bool active = false;
            bool onceActive = false;
            int cycleTime = 0;
    };
    KickerC Kicker;

private:
    double newSpeed;
    int Dir0;
    int Dir1;
    int Dir2;
    int Dir3;
    int Dir_A;
    double Error ;
    double Vel_D[4];
    double Vel[4];
    int speed;
    double direction;
};

extern RobotC Robot;