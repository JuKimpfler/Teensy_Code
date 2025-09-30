#pragma once
#include "INA219.h"
#include "Elementar.h"

class INAC{
    private:
    public:
        void init();
        float Voltage();
        float Voltage_DR();
};

extern INAC INA;