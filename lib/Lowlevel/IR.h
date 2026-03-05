#pragma once
#include "Elementar.h"
#include "Expander.h"
#include "ir_ring.h"

class IRC {
    private:
    
    public:
        uint16_t IR_Values[16];
        double TSSP;
        double DistFaktor = 0.12;
        double visable_th = 0;
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