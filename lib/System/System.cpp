#include "System.h"

SystemC System;

void setColorID(bool ID){
    if(ID == true){ // weis
        US_Back = US_Back_w;
        US_Left = US_Left_w;
        US_Right = US_Right_w;
        US_Front = US_Front_w;
    }
    else{ // schwarz
        US_Back = US_Back_s;
        US_Left = US_Left_s;
        US_Right = US_Right_s;
        US_Front = US_Front_s;
    }
}

void SystemC::begin(bool CIndex){
    setColorID(CIndex);
    PFU.begin();
    Motor.init();
    BNO055.init();
    BC.begin(UART_Pixy);
    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);
    IR.init();
    LDR.init();
    RGB.init();
}

void SystemC::UpdateC::Interface(){
    Expander.I2C.read(I2C_ITF_Main);

    System.Button[0] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT0);
    System.Button[1] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT1);
    System.Button[2] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT2);
    System.Button[3] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT3);

    System.Switches[0] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW0);
    System.Switches[1] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW1);
    System.Switches[2] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW2);

    System.Start = digitalRead(Start_Port);
}

void SystemC::UpdateC::Sensors(){
    BC.process();
    IR.read();
    BNO055.read();
    Line.read_Fast();
}

void SystemC::UpdateC::Calculations(){
    PFU.Update();
    PID.Calculate();
    BallCalc.CalcAngle();
    BallCalc.CalcDist();
}
