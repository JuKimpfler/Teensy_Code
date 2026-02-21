#include "INA.h"
INAC INA;


INA219 INA219_lib(I2C_INA219);

void INAC::init(){
    INA219_lib.begin();
}

float INAC::readVoltage(){
    return INA219_lib.getBusVoltage();
}
