#pragma once
#include "Elementar.h"
#include "Expander.h"
#include "IR_ring.h"

class IRC {
    private:
    
    public:
        double Distance_raw;
        //double NullCall  = 100;
        double Distance_raw2;
        double Angle;
        double Distance;
        double IR_Values[16];
        double TSSP;
        double DistCal;
        double DistFaktor;

        void read();
        int GetData(int Port);
        int GetTSSP();
        void init();
        void Calib_Offset();
        void Calib_Dist();

};

class BallC{
    public:
        double Angle;
        double Distance;
        bool inSight;
    private:
};

extern BallC Ball;
extern IRC IR;