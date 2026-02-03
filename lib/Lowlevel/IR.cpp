#include "IR.h"

IRC IR;
BallC Ball;

I2C_bus i2c1(I2C_BUS);
Device_handle ir_ring_handle = {i2c1, I2C_target{0x0A}};
IR_ring ir_lib(ir_ring_handle);

void IRC::init(){
    I2C_BUS.begin();
    I2C_BUS.setClock(1000000);
}

int IRC::GetTSSP(){
    return 0;
}

void IRC::read(){
    uint16_t Values[16];
    ir_lib.read_calibrated_values(Values);
    for ( int i = 0 ; i<16 ; i++){
        IR_Values[i] = Values[i] - (ir_lib.offset_save[i]);
    } 
}

void IRC::Calib_Dist(){
    DistFaktor = 50/Distance_raw2;
}

void IRC::Calib_Offset(){
    uint16_t Values2[16];
    ir_lib.read_raw_values(Values2);
    for ( int i = 0 ; i<16 ; i++){
        ir_lib.offset_save[i] = Values2[i];
    }
}

int IRC::GetData(int Port){ // raw Data of the IR Sensors
    return IR_Values[Port];
}