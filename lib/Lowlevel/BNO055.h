#pragma once
#include "Elementar.h"

class BNO055C {

    private:
    public:

        float BNO_Cal;
        float TiltZ;

        void init();
        void read();
        void Calibrate();
        void showCal();
};

extern BNO055C BNO055;