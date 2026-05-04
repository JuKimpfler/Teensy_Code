#include "System.h"

SystemC System;

elapsedMillis Interface_Timer;
elapsedMillis Schuss_Timer;
elapsedMicros Cycle_Timer;

void setColorID(bool ID){
    if(ID == true){ // weis
        US_Back = US_Back_w;
        US_Left = US_Left_w;
        US_Right = US_Right_w;
    }
    else{ // schwarz
        US_Back = US_Back_s;
        US_Left = US_Left_s;
        US_Right = US_Right_s;
    }
}

void SystemC::begin(bool CIndex){
    setColorID(CIndex);
    Motor.init();
    Line.init();
    BNO055.init();
    BC.begin(UART_Pixy);
    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);
    IR.init();
    LDR.init();
    RGB.init();
    US.init();
}

void SystemC::UpdateC::Interface(){
    RGB.Apply();
    Expander.I2C.read(I2C_ITF_Main);

    System.Button[0] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT0);
    System.Button[1] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT1);
    System.Button[2] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT2);
    System.Button[3] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT3);

    System.Switches[0] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW0);
    System.Switches[1] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW1);
    System.Switches[2] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW2);
}

void SystemC::UpdateC::Sensors(){
    System.Start = digitalRead(Start_Port);
    IR.read();
    BNO055.read();
    Line.read_Fast();
    Robot.Kicker.Update();
    US.read();
    Cam.Update();
}

void SystemC::UpdateC::Calculations(){
    PID.Calculate();
    BallCalc.CalcAngle();
    LineCalc.Calc();
}
