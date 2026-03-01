#include "IR.h"

IRC IR;
BallC Ball;

I2C_bus i2c1(I2C_BUS);
Device_handle ir_ring_handle = {i2c1, I2C_target{0x0A}};
IR_ring ir_lib(ir_ring_handle);

// init

void IRC::init(){
    if(Robo_NR=="s"){
        //ir_lib.write_ir_sensor_mask(0b1111111111111110);
        ir_lib.write_offsets(mini);
        ir_lib.write_gains(maxi);
    }
}

void IRC::read(){               //01234567890123456
    
    if(Robo_NR=="s"){
        float ValueF;
        ValueF=U.Circel(((ir_lib.read_ball_angle()*-1) -180)-6);
        Ball.Angle = ValueF;

        //ir_lib.read_calibrated_values(IR_Values);
    }
    else{
        float ValueF;
        ValueF=U.Circel(ir_lib.read_ball_angle()*-1);
        Ball.Angle = ValueF;
    }

    uint16_t Value;
    Value=ir_lib.read_ball_distance();
    Ball.Distance_raw = Value;

    Ball.inSight = !(Ball.Distance_raw < 35);
}

void IRC::Calib_Dist(){
    ir_lib.write_ball_visibility_threshold(ir_lib.read_ball_distance());
}