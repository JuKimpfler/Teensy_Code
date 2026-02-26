#include "BNO055.h"
#include "RGB.h"
#include "BNO055_beaver.h"
#include "Debug.h"

BNO055C BNO055;
BNO055_beaver BNO_beaver(0x28,&Wire1);

void BNO055C::init(){

    // Unit Select
    uint8_t buffer[2] = {0, 0};
    Wire1.beginTransmission(0x28);
    buffer[0] = REG_UNIT_SEL;
    // Einheiten festlegen (BNO055_beaver.pdf S.69)
    // Bit7: Data output format = 1 -> Android (verändert Ausgabe von Pitch-Werten)
    // Bit4: Temperature = 0 -> Celsius
    // Bit2: Euler units = 0 -> Grad (Winkeleinheit)
    // Bit1: Angular Rate units = 0 -> Dps (Rotationsgeschwindigkeit Grad/s)
    // Bit0: Acceleration units = 0 -> m/s² (Beschleunigung)
    buffer[1] = (1 << 7) | (0 << 4) | (0 << 2) | (0 << 1) | (1 << 0);
    Wire1.write(buffer, 2);

    Wire1.write(REG_OPR_MODE);
    Wire1.write(0x09);
    Wire1.endTransmission();
}

void BNO055C::read(){
    Wire1.beginTransmission(0x28);
    Wire1.write(0x1A);
    Wire1.endTransmission();
    Wire1.requestFrom(0x28, 2);
    uint8_t ret[2];
    Wire1.readBytes(ret, 2);
    int16_t temp = (((int16_t) (ret[1]) << 8) | ret[0]);
    TiltZ = U.Circel(temp-BNO_Cal);
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