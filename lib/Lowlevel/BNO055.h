#pragma once
#include "Elementar.h"
#include "..\Extern\BNO055\BNO055_beaver.h"
#include "Debug.h"

class BNO055C {

    private:
    public:

        int16_t BNO_Cal;
        int16_t TiltZ;
        int16_t Diff;
        double last;

        void init();
        void read();
        float giveRad();
        float giveDeg();
        void Calibrate();
};

extern BNO055C BNO055;