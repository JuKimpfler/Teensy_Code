#include "BNO055.h"
#include "RGB.h"
#include "BNO055_beaver.h"
#include "Debug.h"

//Adafruit_BNO055 bno(55, 0x28, &Wire1);

BNO055C BNO055;

BNO055_beaver BNO_beaver(0x28,&Wire1);

void BNO055C::init(){
    //bno.begin();
    //bno.setExtCrystalUse(true);

    BNO_beaver.init();
}

void BNO055C::read(){
    float Comp_Dir = BNO_beaver.eulHeading();
    Comp_Dir = Comp_Dir-BNO_Cal;
    if (Comp_Dir<0){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    if(Comp_Dir<=180){Comp_Dir=Comp_Dir;}
    else if (Comp_Dir>180){Comp_Dir=Comp_Dir-360;}
    if (Comp_Dir<-180){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    TiltZ = Comp_Dir;
}

void BNO055C::Calibrate(){
    BNO_Cal = BNO_beaver.eulHeading();;
}

void BNO055C::showCal(){
    int mag = 0;
    mag = BNO_beaver.calibStatus();

    if (mag==0){RGB.write(0,"R");}// LED AN
    else if (mag==1){RGB.write(0,"O");}// LED AN
    else if (mag==2){RGB.write(0,"Y");}// LED AN
    else if (mag==3){RGB.write(0,"G");}// LED AN
}