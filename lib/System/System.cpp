#include "System.h"

SystemC System;

elapsedMillis Interface_Timer;
elapsedMillis Schuss_Timer;
elapsedMicros Cycle_Timer;

void SystemC::Start_Update(){
    Start = digitalReadFast(Start_Port);
}

void SystemC::Button_Update(){
    Expander.I2C.read(I2C_ITF_Main);

    Button[0] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT0);
    Button[1] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT1);
    Button[2] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT2);
    Button[3] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT3);

    Switches[0] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW0);
    Switches[1] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW1);
    Switches[2] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW2);
    
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

    //Mouse.init();

    IR.init();

    Expander.ADC.init(CS_LineA);
    Expander.ADC.init(CS_LineB);
    Expander.ADC.init(CS_LineC);
    Expander.ADC.init(CS_LineD);
    Expander.ADC.init(CS_LineVW);

    LDR.init();

    BNO055.init();

    US.init();
}

void SystemC::UpdateC::Interface(){
    if (Interface_Timer > 1000/Interface_Frequency){
        System.Button_Update();
        Interface_Timer=0;
    }
}

void SystemC::UpdateC::Sensors(){

    //Mouse.read();

    Cam.read();

    System.Start_Update();

    Expander.ADC.read(CS_LineA);
    Expander.ADC.read(CS_LineB);
    Expander.ADC.read(CS_LineC);
    Expander.ADC.read(CS_LineD);
    Expander.ADC.read(CS_LineVW);

    IR.read();

    Line.read();
    Line.read_VW();

    BNO055.read();

    US.read();

    if(digitalReadFast(Start_Port)){BL.doRolle();}
}

void SystemC::UpdateC::Calculations(){
    LineCalc.Calc();
    PID.Calculate();
    BallCalc.getAngle(Ball.Angle,Ball.Distance);
    BallCalc.CalcAngle();
    BallCalc.CalcDist();
    CamCalc.Angle();
}