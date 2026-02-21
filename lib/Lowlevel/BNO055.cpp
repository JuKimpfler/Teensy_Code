#include "BNO055.h"
#include "RGB.h"

Adafruit_BNO055 bno(55, 0x28, &Wire1);

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

void BNO055C::showCal(){
    uint8_t system2 = 0;
    uint8_t gyro = 0;
    uint8_t accel = 0;
    uint8_t mag = 0;
    bno.getCalibration(&system2, &gyro, &accel, &mag);

    if (mag==0){RGB.write(0,"R");}// LED AN
    else if (mag==1){RGB.write(0,"O");}// LED AN
    else if (mag==2){RGB.write(0,"Y");}// LED AN
    else if (mag==3){RGB.write(0,"G");}// LED AN
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