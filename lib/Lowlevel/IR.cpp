#include "IR.h"

IRC IR;
BallC Ball;

I2C_bus i2c1(I2C_BUS);
Device_handle ir_ring_handle = {i2c1, I2C_target{0x0A}};
IR_ring ir_lib(ir_ring_handle);

// init

void IRC::init(){
    ir_lib.write_offsets(IR_mini);
    ir_lib.write_gains(IR_maxi);
}

void IRC::read(){               //01234567890123456
    
    float ValueF;
    ValueF=U.Circel(ir_lib.read_ball_angle()*-1);
    Ball.Angle = ValueF;

    uint16_t Value;
    Value=ir_lib.read_ball_distance();
    Ball.Distance_raw = Value;

    Ball.inSight = !(Ball.Distance_raw < IR_Sight);
}