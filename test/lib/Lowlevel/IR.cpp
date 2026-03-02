#include "IR.h"

IRC IR;
BallC Ball;

I2C_bus i2c1(I2C_BUS);
Device_handle ir_ring_handle = {i2c1, I2C_target{0x0A}};
IR_ring ir_lib(ir_ring_handle);

/**
 * @brief Initialize the IR class
 * @details This function initializes the IR class by starting the I2C bus and setting the clock speed to 1000000 Hz.
 */
void IRC::init(){
    I2C_BUS.begin();
    I2C_BUS.setClock(1000000);
}

/**
 * @brief Dummy function for TSSP
 * @details This function is currently not implemented and will return 0.
 * @return 0
 */
int IRC::GetTSSP(){
    return 0;
}

/**
 * @brief Reads the calibrated values from the IR sensors.
 * @details Reads the calibrated values from the IR sensors and stores them in the IR_Values array.
 * This is necessary to correct the raw values of the IR sensors for calculation of the ball distance and angle.
 */
void IRC::read(){
    uint16_t Values[16];
    ir_lib.read_calibrated_values(Values);
    for ( int i = 0 ; i<16 ; i++){
        IR_Values[i] = Values[i] - (ir_lib.offset_save[i]);
    } 
}

/**
 * @brief Calibrates the distance factor of the IR sensors.
 * @details Reads the raw values of the IR sensors and stores them in the DistFaktor variable.
 * This is necessary to correct the raw values of the IR sensors for calculation of the ball distance and angle.
 */
void IRC::Calib_Dist(){
    DistFaktor = 50/Distance_raw2;
}

/**
 * @brief Calibrates the offset values of the IR sensors.
 * @details Reads the raw values of the IR sensors and stores them in the offset_save array.
 * This is necessary to correct the raw values of the IR sensors for calculation of the ball distance and angle.
 */
void IRC::Calib_Offset(){
    uint16_t Values2[16];
    ir_lib.read_raw_values(Values2);
    for ( int i = 0 ; i<16 ; i++){
        ir_lib.offset_save[i] = Values2[i];
    }
}

/**
 * @brief Returns the raw data of the IR sensors.
 * @details This function returns the raw data of the IR sensors, which can be used to calculate the ball distance and angle.
 * @param Port The port number of the IR sensor.
 * @return The raw data of the IR sensor at the specified port.
 */
int IRC::GetData(int Port){ // raw Data of the IR Sensors
    return IR_Values[Port];
}