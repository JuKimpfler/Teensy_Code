#pragma once
#include "INA219.h"
#include "Elementar.h"

class INAC{
    private:
    public:
        void init();
        float readVoltage();
};

extern INAC INA;