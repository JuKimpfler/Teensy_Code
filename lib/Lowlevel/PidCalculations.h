#pragma once
#include "Elementar.h"
#include "BNO055.h"

class PIDC {

    private:
        double last;
        double now;
        float Angle;

    public:
        float esum;
        float ealt;  
        double diffTime;
        float Out;

        void Calculate();
        void setAngle(float Angle1);
};

extern PIDC PID;