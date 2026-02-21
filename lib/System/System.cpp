#include "System.h"

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

    // configure kicker output
    pinMode(Kicker_Port, OUTPUT);

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

    Mouse.read();

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

    //if(digitalReadFast(Start_Port)){BL.doRolle();}
}

void SystemC::UpdateC::Calculations(){
    LineCalc.Calc();
    PID.Calculate();
    BallCalc.getAngle(Ball.Angle,Ball.Distance);
    BallCalc.CalcAngle();
    BallCalc.CalcDist();
}