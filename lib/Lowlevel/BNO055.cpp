#include "BNO055.h"

Adafruit_BNO055 bno;

BNO055C BNO055;

void BNO055C::init(){
    bno.begin();
    bno.setExtCrystalUse(true);
}

void BNO055C::read(){
    sensors_event_t event; 
    bno.getEvent(&event);

    float Comp_Dir = event.orientation.x;
    Comp_Dir = Comp_Dir-BNO_Cal;
    if (Comp_Dir<0){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    if(Comp_Dir<=180){Comp_Dir=Comp_Dir;}
    else if (Comp_Dir>180){Comp_Dir=Comp_Dir-360;}
    if (Comp_Dir<-180){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    TiltZ = Comp_Dir;

    Comp_Dir = event.orientation.y;
    if (Comp_Dir<0){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    if(Comp_Dir<=180){Comp_Dir=Comp_Dir;}
    else if (Comp_Dir>180){Comp_Dir=Comp_Dir-360;}
    if (Comp_Dir<-180){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    TiltX = Comp_Dir;

    Comp_Dir = event.orientation.z;
    if (Comp_Dir<0){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    if(Comp_Dir<=180){Comp_Dir=Comp_Dir;}
    else if (Comp_Dir>180){Comp_Dir=Comp_Dir-360;}
    if (Comp_Dir<-180){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    TiltY = Comp_Dir;
}

void BNO055C::Calibrate(){
    sensors_event_t event; 
    bno.getEvent(&event);
    BNO_Cal = event.orientation.x;
}

float BNO055C::give_TiltX(){
    return TiltX;
}

float BNO055C::give_TiltY(){
    return TiltY;
}

float BNO055C::give_TiltZ(){
    return TiltZ;
}