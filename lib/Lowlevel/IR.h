#pragma once
#include "Elementar.h"
#include "Expander.h"
#include "ir_ring.h"

class IRC {
    private:
    
    public:
        uint16_t IR_Values[16];
        double TSSP;
        double DistCal = 78;
        double DistFaktor = 0.12;
        double visable_th = 0;
        const int16_t mini[16] = {16,24,15,17,19,17,16,18,14,17,14,15,54,17,16,0};
        const uint16_t maxi[16] = {2920,2920,2854,2968,2958,3016,2985,2968,2664,2713,2935,2279,2914,2873,2850,56};

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
        uint16_t Distance;
        uint16_t Distance_raw;
        uint16_t Distance_raw2;
        bool inSight;
        int Angle_P2; // Ballwinkel von Player2 
        int Distance_P2; // Balldistance von Player2 
    private:
};

extern BallC Ball;
extern IRC IR;