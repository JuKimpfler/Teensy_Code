#pragma once
#include "Elementar.h"
#include "BNO055.h"

class PIDC {

    private:
        double last;
        double now;
        double Angle;

    public:
        double esum;
        double ealt;  
        double diffTime;
        double Out;

        void Calculate();
        void setAngle(double Angle1);
};

extern PIDC PID;