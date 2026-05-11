#include "IR.h"
#include "Debug.h"

IRC IR;
BallC Ball;

I2C_bus i2c1(I2C_BUS);
Device_handle ir_ring_handle = {i2c1, I2C_target{0x0A}};
IR_ring ir_lib(ir_ring_handle);

// init

void IRC::init(){
    if(Color_ID){ir_lib.write_offsets(IR_mini_w_calib);}
    else{ir_lib.write_offsets(IR_mini_s_calib);}
    if(Color_ID){ir_lib.write_gains(IR_maxi_w_calib);}
    else{ir_lib.write_gains(IR_maxi_s_calib);}
    
    ir_lib.write_ir_sensor_mask(0b1111111111111111);
}

void IRC::read(){
    ir_lib.read_calibrated_values(IR_Values);
    
    float ValueF;
    ValueF=U.Circel(ir_lib.read_ball_angle()*-1);
    Ball.Angle_raw = ValueF;

    int Position1 = 0;
    int max = IR.IR_Values[0];
    for (int i = 0;i<16;i++){
        if(IR.IR_Values[i]>max){
            Position1 = i;
            max = IR.IR_Values[i];
        }
    }
    int Position2 = 0;
    max = IR.IR_Values[0];
    for (int i = 0;i<16;i++){
        if(i != Position1){
            if(IR.IR_Values[i]>max){
                Position2 = i;
                max = IR.IR_Values[i];
            }
        }
    }

    Ball.Distance_raw = (IR.IR_Values[Position1])/5.9;
    Ball.Distance_raw2 = ((1/sqrt(Ball.Distance_raw)) * 2000);
}

void IRC::Calib_Offset(){
    for (int i = 0;i<16;i++){
        if (IR_mini_conf[i]<ir_lib.read_raw_value(i)){
            IR_mini_conf[i]=ir_lib.read_raw_value(i);
        }
        if (IR_maxi_conf[i]>ir_lib.read_raw_value(i)){
            IR_maxi_conf[i]=ir_lib.read_raw_value(i);
        }
    }
}

void IRC::Calib_Dist(){
    Dist_Offset = Ball.Distance_raw2;
    Angle_Offset = Ball.Angle_raw;
}

