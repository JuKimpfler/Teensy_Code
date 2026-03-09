#include "IR.h"

IRC IR;
BallC Ball;

I2C_bus i2c1(I2C_BUS);
Device_handle ir_ring_handle = {i2c1, I2C_target{0x0A}};
IR_ring ir_lib(ir_ring_handle);

// init

void IRC::init(){
    #ifdef Robo_w
        ir_lib.write_offsets(IR_mini_w);
        ir_lib.write_gains(IR_maxi_w);
        ir_lib.write_ir_sensor_mask(0b1111111111111111);
    #endif
    #ifdef Robo_s
        ir_lib.write_offsets(IR_mini_s);
        ir_lib.write_gains(IR_maxi_s);
        ir_lib.write_ir_sensor_mask(0b1111111111111111);
    #endif
}

void IRC::read(){
    #ifdef Ir_Calib    
        ir_lib.read_raw_values(IR_Values_raw);
    #endif
    ir_lib.read_calibrated_values(IR_Values);
    
    float ValueF;
    ValueF=U.Circel(ir_lib.read_ball_angle()*-1);
    Ball.Angle_raw = ValueF;

    uint16_t Value=0;
    //for (int i = 0; i<16 ; i++){
    //    Value=IR_Values[i]+Value;
    //}
    Value = ir_lib.read_ball_distance();
    Ball.Distance_raw = Value;

    Ball.inSight = !(Ball.Distance_raw < IR_Sight);
}

void IRC::Calib_Dist(){
    Dist_Offset = Ball.Distance_raw2;
    Angle_Offset = Ball.Angle_raw;
}