#pragma once
#include "Adafruit_BNO055.h"
#include "Elementar.h"

class BNO055C {

    private:
    public:

        float BNO_Cal;

        float TiltX;
        float TiltY;
        float TiltZ;

        void init();
        void read();
        void Calibrate();
        float give_TiltZ();
        float give_TiltX();
        float give_TiltY();
};

extern BNO055C BNO055;