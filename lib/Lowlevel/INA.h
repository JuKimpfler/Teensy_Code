#pragma once
#include "..\Extern\INA219\INA219.h"
#include "Elementar.h"

class INAC{
    private:
        int Summe;
        int filter[6];
    public:
        void init();
        float Voltage();
        float Voltage_DR();
        float Current();
        float Current_DR();
};

extern INAC INA;