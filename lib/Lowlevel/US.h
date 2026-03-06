#pragma once
#include "Elementar.h"
#include "Wire.h"

class USC{
    private:
    public:
        void init();
        void read();

        //0 = links, 1 = hinten, 2 = rechts
        double Distance_raw[3];
        double Distance;
};

extern elapsedMillis US_Timer1;
extern USC US ;