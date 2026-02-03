#pragma once
#include "Elementar.h"
#include "Expander.h"
#include "IR_ring.h"

class IRC {
    private:
    
    public:
        float Distance_raw;
        //float NullCall  = 100;
        float Distance_raw2;
        float Angle;
        float Distance;
        float IR_Values[16];
        float TSSP;
        float DistCal;
        float DistFaktor;

        void read();
        int GetData(int Port);
        int GetTSSP();
        void init();
        void Calib_Offset();
        void Calib_Dist();

};

class BallC{
    public:
        float Angle;
        float Distance;
        bool inSight;
    private:
};

extern BallC Ball;
extern IRC IR;