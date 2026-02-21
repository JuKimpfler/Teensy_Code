#pragma once
#include "Elementar.h"
#include <Adafruit_NeoPixel.h>   // for RGB LEDs

class RGBC{
    private:
        void Apply();
    public:
        int G[3]={0,0,0};
        int B[3]={0,0,0};
        int R[3]={0,0,0};                    
        void init();
        void write(int Pos, String color);
};

extern RGBC RGB;
 