#include "BNO055.h"

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

float BNO055C::giveDeg(){
    return TiltZ;
}

float BNO055C::giveRad(){
    return TiltZ*DEG_TO_RAD;
}