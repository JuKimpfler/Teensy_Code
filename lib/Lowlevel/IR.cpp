#include "IR.h"
#include "Debug.h"

IRC IR;
BallC Ball;

I2C_bus i2c1(I2C_BUS);
Device_handle ir_ring_handle = {i2c1, I2C_target{0x0A}};
IR_ring ir_lib(ir_ring_handle);

// init

void IRC::init(){
    ir_lib.write_offsets(IR_mini_s);
    ir_lib.write_gains(IR_maxi_s);
    ir_lib.write_ir_sensor_mask(0b1111111111111111);
    //ir_lib.load_calibration(true,true,false);
}

void IRC::read(){
    #ifdef Ir_Calib    
        ir_lib.read_gains(gains);
        ir_lib.read_offsets(offsets);
    #endif
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

    uint16_t Value=0;
    //for (int i = 0; i<16 ; i++){
    //    Value=IR_Values[i]+Value;
    //}
    //Value = ir_lib.read_ball_distance();
    //Ball.Distance_raw = Value;

    //Ball.inSight = !(Ball.Distance_raw < IR_Sight);
}

void IRC::Calib_Offset(){
    for (int i = 0;i<16;i++){
        if (ir_lib.read_raw_value(i)>ir_lib.read_gain(i)){
            ir_lib.write_gain(i,ir_lib.read_raw_value(i));
        }
        if (ir_lib.read_raw_value(i)<ir_lib.read_offset(i)){
            ir_lib.write_offset(i,ir_lib.read_raw_value(i));
        }
    }
    
}

void IRC::Save(){
    Debug.Start();
    String gains_S ;
    for (int i =0;i<15;i++){
        gains_S = gains_S+String(gains[i])+"," ;
    }
    gains_S = gains_S+String(gains[15]) ;
    Debug.Plot("gains",gains_S);
    
    String offsets_S ;
    for (int i =0;i<15;i++){
        offsets_S = offsets_S+String(offsets[i])+"," ;
    }
    offsets_S = offsets_S+String(offsets[15]) ;
    Debug.Plot("offsets",offsets_S);
    Debug.Send();
}

void IRC::Calib_Dist(){
    Dist_Offset = Ball.Distance_raw2;
    Angle_Offset = Ball.Angle_raw;
}

