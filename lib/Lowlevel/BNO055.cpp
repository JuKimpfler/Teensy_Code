#include "BNO055.h"
#include "RGB.h"
#include "BNO055_beaver.h"
#include "Debug.h"

BNO055C BNO055;

BNO055_beaver BNO_beaver(0x28,&Wire1);

void BNO055C::init(){
    BNO_beaver.init();
}

void BNO055C::read(){
    last = TiltZ;
    TiltZ = U.Circel(BNO_beaver.eulHeading()-BNO_Cal);
    if (last == 0){
        last = 0.0001;
    }
}

void BNO055C::Calibrate(){
    BNO_Cal = U.Circel(BNO_beaver.eulHeading());
}

void BNO055C::showCal(){
    int mag = 0;
    mag = BNO_beaver.calibStatus();

    if (mag==0){RGB.write(0,"R");}// LED AN
    else if (mag==1){RGB.write(0,"O");}// LED AN
    else if (mag==2){RGB.write(0,"Y");}// LED AN
    else if (mag==3){RGB.write(0,"G");}// LED AN
}