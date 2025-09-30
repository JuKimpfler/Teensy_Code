#pragma once
#include "Elementar.h"
#include "Motor.h"
#include "Expander.h"
#include "PidCalculations.h"

class RobotC {
public:
    void Turn(float Angel ,int Speed = HighSpeed);
    void Drive(float Dir , float Angel = 200000 ,int Speed = HighSpeed);
    void Stop();
    
    class KickerC{
        public:
            void Once();
            void On(int Cycletime);
            void Off();
    };
    KickerC Kicker;

private:
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