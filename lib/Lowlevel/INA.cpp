#include "INA.h"
INAC INA;


INA219 INA219_Main(I2C_INA219_Main);

INA219 INA219_DR(I2C_INA219_DR);

void INAC::init(){
    INA219_Main.begin();
    INA219_DR.begin();
}

float INAC::Voltage_DR(){
    return INA219_DR.getBusVoltage();
}

float INAC::Voltage(){
    return INA219_Main.getBusVoltage();
}
