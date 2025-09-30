#include "System.h"

// XCP
#ifdef XCP_BL
  XcpSxIMaster xcpMaster_bl = XcpSxIMaster(500000,7);
#endif
#ifdef XCP_USB
  XcpSxIMaster xcpMaster_usb = XcpSxIMaster(500000,0);
#endif

#ifdef __cplusplus
extern "C" {
#endif
PROGMEM const char kXcpStationId[kXcpStationIdLength] = "Bodenseekoalas_2vs2_" __DATE__ " "  __TIME__;
#ifdef __cplusplus
}
#endif

SystemC System;

elapsedMillis Interface_Timer;
elapsedMillis US_Timer;
elapsedMillis Schuss_Timer;
elapsedMicros Cycle_Timer;

void SystemC::Start_Update(){
    Start = digitalReadFast(Start_Port);
}

void SystemC::Button_Update(){
    Expander.I2C.read(I2C_Ex);
    Expander.I2C.read(I2C_ITF_1);
    Expander.I2C.read(I2C_ITF_2);

    Button[0] = !Expander.I2C.give(I2C_Ex,BTN1_Ex);
    Button[1] = !Expander.I2C.give(I2C_Ex,BTN2_Ex);
    Button[2] = !Expander.I2C.give(I2C_Ex,BTN3_Ex);
    Button[3] = !Expander.I2C.give(I2C_Ex,BTN4_Ex);
    Switches[0] = !Expander.I2C.give(I2C_Ex,SW1_Ex);
    Switches[1] = !Expander.I2C.give(I2C_Ex,SW2_Ex);
    Switches[2] = !Expander.I2C.give(I2C_Ex,SW3_Ex);

    Switches[3] = !Expander.I2C.give(I2C_ITF_1,SW1_ITF1);
    Switches[4] = !Expander.I2C.give(I2C_ITF_1,SW2_ITF1);
    Switches[5] = !Expander.I2C.give(I2C_ITF_1,SW3_ITF1);
    Switches[6] = !Expander.I2C.give(I2C_ITF_1,SW4_ITF1);
    Switches[7] = !Expander.I2C.give(I2C_ITF_1,SW5_ITF1);
    Start_ITF = !Expander.I2C.give(I2C_ITF_1,Start_ITF1);
    Button[4] = !Expander.I2C.give(I2C_ITF_1,BTN1_ITF1);
    Button[5] = !Expander.I2C.give(I2C_ITF_1,BTN2_ITF1);

    Button[6] = !Expander.I2C.give(I2C_ITF_1,BTN1_ITF2);
    Button[7] = !Expander.I2C.give(I2C_ITF_1,BTN2_ITF2);
}

void SystemC::initC::Motors(){
    Motor.init();
}

void SystemC::initC::Interface(){
    //RGB.init();
    //Debug.begin();
    Expander.I2C.init(I2C_Ex,Ex_config);
    Expander.I2C.init(I2C_ITF_1,ITF1_config);
    Expander.I2C.init(I2C_ITF_2,ITF2_config);
}

void SystemC::initC::Sensors(){
    pinMode(Start_Port,INPUT);

    Expander.ADC.init(CS_Line_OUT);
    Expander.ADC.init(CS_LineA);
    Expander.ADC.init(CS_LineB);
    Expander.ADC.init(CS_LineC);
    Expander.ADC.init(CS_LineD);

    INA.init();

    LDR.init();

    BNO055.init();

    //OpenMV.init();

    //Ultrasonic.init(US_Back);
    //Ultrasonic.init(US_Front);
    //Ultrasonic.init(US_Left);
    //Ultrasonic.init(US_Right);
}

void SystemC::UpdateC::Interface(){
    if (Interface_Timer > 1000/Interface_Frequency){
        System.Button_Update();
        Interface_Timer=0;
    }
}

void SystemC::UpdateC::Sensors(){
    
    System.Start_Update();

    Expander.ADC.read(CS_Line_OUT);
    Expander.ADC.read(CS_LineA);
    Expander.ADC.read(CS_LineB);
    Expander.ADC.read(CS_LineC);
    Expander.ADC.read(CS_LineD);

    IR.read();

    Line.read();
    
    //OpenMV.read();
    PID.Calculate();
    BNO055.read();
    /*
    if (US_Timer > 1000/US_Frequency){
        Ultrasonic.read(US_Back);
        Ultrasonic.read(US_Front);
        Ultrasonic.read(US_Left);
        Ultrasonic.read(US_Right);
        US_Timer = 0;
    }
        */
}

void SystemC::UpdateC::Calculations(){
    LineCalc.Calc();
}