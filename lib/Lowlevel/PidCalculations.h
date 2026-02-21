#pragma once
#include "Elementar.h"
#include "BNO055.h"

class PIDC {

    private:
        float last;
        float now;
        float Angle;

    public:
        float esum;
        float ealt;  
        float diffTime;
        float Out;

        void Calculate();
        void setAngle(float Angle1);
};

extern PIDC PID;