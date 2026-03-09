#pragma once
#include "Elementar.h"

class CamC{
    private:
        void Decode(String message);
    public:
        int x;
        int y;
        int area;
        int rest;
        bool goal; // true = gelb , false = blau                    
        String message;
        void read();
};

extern CamC Cam;

class GoalC{
    public:
        double Angle;
        double Distance;
        double X;
        double Y;
        double Area;
        bool inSight;
        bool Regeln;
        bool lastdir; // true = - / false = +
    private:
};

extern GoalC Goal;
 