#pragma once
#include "Elementar.h"

class BNO055C {

    private:
    public:

        int16_t BNO_Cal;
        int16_t TiltZ;

        void init();
        void read();
        void Calibrate();
        void showCal();
};

extern BNO055C BNO055;