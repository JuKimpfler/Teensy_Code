#pragma once
#include "Elementar.h"
#include "Motor.h"
#include "Expander.h"
#include "PidCalculations.h"
#include "IR.h"
#include "Cam.h"

extern elapsedMicros Drive_Smoothed_Timer;

class RobotC {
public:
    int Drive_Smoothed_aktive = 0;
    float Drive_Smoothed_start = 0;
    float Drive_Smoothed_optimal = 0;
    float Drive_Smoothed_Dir = 0;
    float Drive_Smoothed_Angle = 0; 
    float Drive_Smoothed_last = 0;
    float Drive_Smoothed_diff = 0; 
    float Drive_Smoothed_diffnow = 0;

    void Turn(float Angel);

    void Drive_Smoothed_set(float Dir , float Angle , int sollSpeed , int istSpeed);
    void Drive_Smoothed_Update();
    void Drive_Smoothed_Kill();

    void Drive(float Dir , float Angel = 200000 ,int Speed1 = HighSpeed);
    void Stop();
    void Break();
    
    class KickerC{
        public:
            void Once();
            void On(int Cycletime);
            void Off();
    };
    KickerC Kicker;

private:
    float newSpeed;
    int Dir0;
    int Dir1;
    int Dir2;
    int Dir3;
    int Dir_A;
    float Error ;
    float Vel_D[4];
    float Vel[4];
    int speed;
    float direction;
};

extern RobotC Robot;