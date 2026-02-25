#include "System.h"
#include "Debug.h"
#include "Game_Thread.h"

SystemC System;

elapsedMillis Interface_Timer;
elapsedMillis Schuss_Timer;
elapsedMicros Cycle_Timer;

void SystemC::Start_Update(){
    System.Start = digitalRead(Start_Port);
}

void SystemC::Button_Update(){
    Expander.I2C.read(I2C_ITF_Main);

    System.Button[0] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT0);
    System.Button[1] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT1);
    System.Button[2] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT2);
    System.Button[3] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT3);

    System.Switches[0] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW0);
    System.Switches[1] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW1);
    System.Switches[2] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW2);
    
}

void SystemC::initC::Motors(){
    Motor.init();
}

void SystemC::initC::Interface(){
    RGB.init();
    Expander.I2C.init(I2C_ITF_Main,Input_Mode);
}

void SystemC::initC::Sensors(){
    pinMode(Start_Port,INPUT);

    Mouse.init();

    IR.init();

    pinMode(Kicker_Port, OUTPUT);

    LDR.init(); 

    BNO055.init(); 

    //US.init(); 

    Line.init();

    BNO055.Calibrate();
}

void SystemC::UpdateC::Interface(){
    if (Interface_Timer > 1000/Interface_Frequency){
        System.Button_Update();
        Interface_Timer=0;
    }
}

void SystemC::UpdateC::Sensors(){
    System.Start_Update(); // p1
    Line.read_Fast(); // p1

    if (Cycle_P2 > 5){
        US.read(); // p2
        IR.read(); 
        Mouse.read(); // P2
        US.read(); // p2
        BallCalc.getAngle();
        BallCalc.CalcAngle();
        BallCalc.CalcDist();
        PID.Calculate();
        BNO055.read(); // p2
        Cam.read(); // P3
        Cycle_P2 = 0;
    }

    if (Cycle_P3 > 10){
        Debug.Start();
        Debug.Plot("Ball_Angle",Ball.Angle);
        Debug.Plot("Ball_dist",Ball.Distance);
        Debug.Plot("Drive",BallCalc.DriveAngle);
        //Debug.Plot("Line_min",Line.min_schwelle);
        //Debug.Plot("Line_max",Line.max_schwelle);
        //Debug.Plot_List("Line",Line.lineVW,8);
        Debug.Send();
        Cycle_P3 = 0;
    }

    LineCalc.Calc();
    //if(digitalReadFast(Start_Port)){BL.doRolle();}
}