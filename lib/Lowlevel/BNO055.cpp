#include "BNO055.h"
#include "RGB.h"
#include "BNO055_beaver.h"
#include "Debug.h"

BNO055C BNO055;

BNO055_beaver BNO_beaver(0x28,&Wire1);

void BNO055C::init(){
    BNO_beaver.init();
    Wire1.beginTransmission(0x28);
    Wire1.write(0x1A);
    Wire1.endTransmission();
}

void BNO055C::read(){
    Wire1.setClock(800000);
    float Comp_Dir = BNO_beaver.eulHeading();
    Comp_Dir = Comp_Dir-BNO_Cal;
    if (Comp_Dir<-180){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>180){Comp_Dir = Comp_Dir-360;}
    TiltZ = Comp_Dir;
    Wire1.setClock(1000000);
}

void BNO055C::Calibrate(){
    Wire1.setClock(200000);
    BNO_Cal = BNO_beaver.eulHeading();
    Wire1.setClock(1000000);
}

void BNO055C::showCal(){
    int mag = 0;
    mag = BNO_beaver.calibStatus();

    if (mag==0){RGB.write(0,"R");}// LED AN
    else if (mag==1){RGB.write(0,"O");}// LED AN
    else if (mag==2){RGB.write(0,"Y");}// LED AN
    else if (mag==3){RGB.write(0,"G");}// LED AN
}