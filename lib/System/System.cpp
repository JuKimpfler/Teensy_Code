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

void SystemC::init(){
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);

    pinMode(Start_Port,INPUT);
    pinMode(Kicker_Port, OUTPUT);
    LDR.init(); 

    Line.init();
    //Mouse.init();

    Motor.init();
    RGB.init();
    BNO055.init(); 
    
    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);
    US.init();

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
    //US.read(); // 1 micro
    //Mouse.read(); // 177 micros
    //Cam.read(); // 1 micro
    Robot.Kicker.Update(); // 1 micro
    LineCalc.Calc(); // 15 micro
    BallCalc.CalcDist(); // 1 micro
    BallCalc.getAngle(); // 1 micro
    PID.Calculate(); // 1 micro
    BNO055.read(); // 100-500 micro
    IR.read(); // 300 micro
    BL.doRolle();

    if ((Cycle_P3 > 20) && (true)){
        Debug.Start();
        Debug.Plot("ball angle",Ball.Angle);
        Debug.Plot("ball dist",Ball.Distance);
        Debug.Plot("ball dist 2",Ball.Distance_P2);
        Debug.Plot("G-Angle",Goal.Angle);

        Debug.Plot("USL", US.Distance_raw[0]);
        
        Debug.Send();

        RGB.Apply(); // 300
        Cycle_P3 = 0;
    }

    /*if(Reg_Timer>1000){
        Wire1.beginTransmission(0x28);
        Wire1.write(0x1A);
        Wire1.endTransmission();
        Reg_Timer=0;
    }*/
}