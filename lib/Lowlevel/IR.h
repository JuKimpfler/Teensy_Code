#pragma once
#include "Elementar.h"
#include "Expander.h"
#include "IR_ring.h"

class IRC {
    private:

    public:
        float Distance_raw;
        float NullCall  = 100;
        float Angle;
        float Distance;
        int IR_Values[16] ;
        float TSSP;

        void read();
        int GetData(int Port);
        int GetTSSP();
        void init();

};

extern IRC IR;