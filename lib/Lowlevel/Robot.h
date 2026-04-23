#pragma once
#include "Elementar.h"
#include "Motor.h"
#include "Expander.h"
#include "PID.h"

class RobotC {
public:
    void Turn(float Angel ,int Speed = HighSpeed);
    void Drive(float Dir , float Angel,int Speed);
    void Stop();
    void Break();   
    void Turn_Drachen();
    
    class KickerC{
        public:
            void Once(); // single pulse
            void On(int Cycle = 400); // start pulse cycling
            void Off(); // stop cycling
            void Update(); // internal timer handling
            void Update_End();
        private:
            bool active = false;
            bool onceActive = false;
            int cyclet = 0;
            bool set=false;
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