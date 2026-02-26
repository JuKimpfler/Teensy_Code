#include "IR.h"

IRC IR;
BallC Ball;

I2C_bus i2c1(I2C_BUS);
Device_handle ir_ring_handle = {i2c1, I2C_target{0x0A}};
IR_ring ir_lib(ir_ring_handle);

void IRC::read(){
    float ValueF;
    ValueF=ir_lib.read_ball_angle()*-1;
    Ball.Angle = ValueF;

    uint16_t Value;
    Value=ir_lib.read_ball_distance();
    Ball.Distance_raw = Value;

    Ball.inSight = ir_lib.read_ball_visible();
}

void IRC::Calib_Dist(){
    ir_lib.write_ball_visibility_threshold(ir_lib.read_ball_distance());
}