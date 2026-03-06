#include "System.h"
#include "Debug.h"
#include "Game_Thread.h"
#include "BL.h"

SystemC System;

elapsedMillis Reg_Timer;
elapsedMillis Schuss_Timer;
elapsedMicros Cycle_Timer;
elapsedMicros Mess_Timer;


void SystemC::Start_Update(){
    System.Start = digitalRead(Start_Port);
}

void SystemC::Button_Update(){
    Expander.I2C.read(I2C_ITF_Main);

    System.Button[0] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT0);
    System.Button[1] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT1);
    System.Button[2] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT2);
    System.Button[3] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT3);

    System.Switches[0] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW0);
    System.Switches[1] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW1);
    System.Switches[2] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW2);
    
}

void SystemC::Calibrate(String NR){
    if(NR == "w"){
        LDR_Schwelle = LDR_Schwelle_w;
        IR_Dist_Offset = IR_Dist_Offset_w;
        U.List_copy(IR_maxi_w,IR_maxi,16);
        U.List_copy(IR_mini_w,IR_mini,16);
    }
    if(NR == "s"){
        LDR_Schwelle = LDR_Schwelle_s;
        IR_Dist_Offset = IR_Dist_Offset_s;
        U.List_copy(IR_maxi_s,IR_maxi,16);
        U.List_copy(IR_mini_s,IR_mini,16);
    }
}

void SystemC::init(){
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);

    pinMode(Start_Port,INPUT);
    pinMode(Kicker_Port, OUTPUT);
    LDR.init(); 

    Line.init();

    Motor.init();
    RGB.init();
    BNO055.init(); 
    
    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);

    IR.init();
}

void SystemC::UpdateC::Interface(){
    if (!System.Start){
        System.Button_Update(); // 588
    }
}

void SystemC::UpdateC::Sensors(){
    System.Start_Update(); // 1 micro
    Line.read_Fast(); // 60 micro
    US.read(); // 1 micro
    Cam.read(); // 1 micro
    Robot.Kicker.Update(); // 1 micro
    LineCalc.Calc(); // 15 micro
    BallCalc.CalcDist(); // 1 micro
    BallCalc.getAngle(); // 1 micro
    PID.Calculate(); // 1 micro
    BNO055.read(); // 100-500 micro
    IR.read(); // 300 micro
    BL.doRolle();

    if ((Cycle_P3 > 60)){
        #ifdef Debug_EN
        #ifdef Ir_Calib // IR_Calibration 
        Debug.Start();
        Debug.Plot_List("IR",IR.IR_Values,16);
        Debug.Send();
        #endif
        #ifdef Calib // Normal_Calibration
        Debug.Start();
        Debug.Plot("IR_Offset",Ball.Distance_raw2);
        Debug.Plot("Dist",Ball.Distance);
        Debug.Plot("Zone",Zone);
        Debug.Plot("LDR_Analog",analogRead(LDR_Port));
        Debug.Plot("Line_G",Line_Grass);
        Debug.Plot("Line_G_VW",Line_Grass_VW);
        Debug.Plot("Line_N",Line_Norm);
        Debug.Plot("Line_N_VW",Line_Norm_VW);
        Debug.Plot("Line_S",Line_Schwelle);
        Debug.Plot("Line_S_VW",Line_Schwelle_VW);
        Debug.Send();
        #endif
        #ifdef Line_Calib
        Debug.Start();
        Debug.Plot_List("Line", Line.lineVW, 8);
        Debug.Send();
        #endif
        #ifndef Ir_Calib // Game_Debug
        #ifndef Calib 
        #ifndef Line_Calib
        Debug.Start();
        Debug.Plot("USL", US.Distance_raw[0]);
        Debug.Plot("USH", US.Distance_raw[1]);
        Debug.Plot("USR", US.Distance_raw[2]);
        Debug.Plot("goal angle",Goal.Angle);
        Debug.Plot("goal dist",Goal.X);
        Debug.Plot("bno",BNO055.TiltZ);
        Debug.Plot("ball angle 2",Ball.Angle_P2);
        Debug.Plot("Rolle",BL.Rolle);
        Debug.Send();
        #endif
        #endif
        #endif

        RGB.Apply(); // 300
        Cycle_P3 = 0;
        #endif 
    }

    /*if(Reg_Timer>1000){
        Wire1.beginTransmission(0x28);
        Wire1.write(0x1A);
        Wire1.endTransmission();
        Reg_Timer=0;
    }*/
}