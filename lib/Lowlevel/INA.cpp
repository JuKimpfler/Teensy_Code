#include "INA.h"
INAC INA;


INA219 INA219_lib(I2C_INA219);

/**
 * @brief Initializes the INA219 library.
 * 
 * This function is called when the INAC object is created.
 * It initializes the INA219 library by calling begin() on the INA219 object.
 */
void INAC::init(){
    INA219_lib.begin();
}

/**
 * @brief Returns the bus voltage measured by the INA219 sensor in volts.
 *
 * @return The bus voltage in volts.
 */
double INAC::readVoltage(){
    return INA219_lib.getBusVoltage();
}
