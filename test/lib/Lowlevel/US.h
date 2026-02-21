#pragma once
#include "Elementar.h"
#include "Wire.h"

class USC{
    private:
    public:
        void init();
        void read();
        double Distance_raw[2];
        double Distance;
};

extern elapsedMillis US_Timer1;
extern USC US ;