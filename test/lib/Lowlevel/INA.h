#pragma once
#include "INA219.h"
#include "Elementar.h"

class INAC{
    private:
    public:
        void init();
        double readVoltage();
};

extern INAC INA;