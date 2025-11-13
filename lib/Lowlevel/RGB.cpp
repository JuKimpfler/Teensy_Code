#include "RGB.h"
RGBC RGB;

Adafruit_NeoPixel RGB_LEDs = Adafruit_NeoPixel(3, 20, NEO_GRB + NEO_KHZ800);

void RGBC::init(){
    RGB_LEDs.begin();
}

void RGBC::write(int Pos,String color){
    if (color == "R"){
        R[Pos]=255;
        G[Pos]=0;
        B[Pos]=0;
    }
    else if (color == "G"){
        R[Pos]=0;
        G[Pos]=255;
        B[Pos]=0;
    }
    else if (color == "B"){
        R[Pos]=0;
        G[Pos]=0;
        B[Pos]=255;
    }
    else if (color == "O"){
        R[Pos]=255;
        G[Pos]=70;
        B[Pos]=0;
    }
    else if (color == "Y"){
        R[Pos]=255;
        G[Pos]=255;
        B[Pos]=30;
    }
    else{
        R[Pos]=0;
        G[Pos]=0;
        B[Pos]=0;
    }
    Apply();
}

void RGBC::Apply(){
    RGB_LEDs.clear();
    RGB_LEDs.setBrightness(30);
    RGB_LEDs.setPixelColor(0, RGB_LEDs.Color(R[0] ,G[0] ,B[0]));
    RGB_LEDs.setPixelColor(1, RGB_LEDs.Color(R[1] ,G[1] ,B[1]));
    RGB_LEDs.setPixelColor(2, RGB_LEDs.Color(R[2] ,G[2] ,B[2]));
    RGB_LEDs.show();
    Serial.println(" R :"+String(R[0])+" G :"+String(G[0])+" B :"+String(B[0]));
    Serial.println(" R1 :"+String(R[1])+" G1 :"+String(G[1])+" B1 :"+String(B[1]));
    Serial.println(" R2 :"+String(R[2])+" G2 :"+String(G[2])+" B2 :"+String(B[2]));
}
