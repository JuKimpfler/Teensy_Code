#pragma once
#include "Adafruit_BNO055.h"
#include "Elementar.h"

class BNO055C {

    private:
    public:

        double BNO_Cal;

        double TiltX;
        double TiltY;
        double TiltZ;

        void init();
        void read();
        void Calibrate();
        void showCal();
        double give_TiltZ();
        double give_TiltX();
        double give_TiltY();
};

extern BNO055C BNO055;