#include "IR.h"

IRC IR;

I2C_bus i2c1(I2C_BUS);
Device_handle ir_ring_handle = {i2c1, I2C_target{0x0A}};
IR_ring ir_ring(ir_ring_handle);

void IRC::init(){
    I2C_BUS.begin();
    I2C_BUS.setClock(1000000);
}

int IRC::GetTSSP(){
    return 0;
}

void IRC::read(){
    uint16_t calib_data[16];
    ir_ring.read_calibrated_values(calib_data);
    for (int i = 0 ; i++ ; i<16){
        IR_Values[i] = calib_data[i];
    }
    Angle = ir_ring.read_ball_angle();
    Distance_raw = ir_ring.read_ball_distance();
    Distance = (Distance_raw);
}

int IRC::GetData(int Port){ // raw Data of the IR Sensors
    return IR_Values[Port];
}