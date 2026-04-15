#pragma once
#include "Elementar.h"
#include "Wire.h"

class USC{
    private:
    public:
        void read();

        int giveNR(int NR); // cm (0=Vorne, 1=Rechts, 2=Hinten, 3=Links)

        int Distance[4] ; // cm (0=Vorne, 1=Rechts, 2=Hinten, 3=Links)
};

extern elapsedMillis US_Timer1;
extern USC US ;