#include "System.h"

SystemC System;

void SystemC::begin(int CIndex){
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
}
