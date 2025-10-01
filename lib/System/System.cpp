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
    Expander.I2C.read(I2C_Button);
    Expander.I2C.read(I2C_Dip_SW);

    Button[0] = !Expander.I2C.give(I2C_Button,4);
    Button[1] = !Expander.I2C.give(I2C_Button,5);
    Button[2] = !Expander.I2C.give(I2C_Button,6);
    Button[3] = !Expander.I2C.give(I2C_Button,7);

    Switches[0] = !Expander.I2C.give(I2C_Button,0);
    Switches[1] = !Expander.I2C.give(I2C_Button,1);
    Switches[2] = !Expander.I2C.give(I2C_Button,2);
    Switches[3] = !Expander.I2C.give(I2C_Button,3);
}

void SystemC::initC::Motors(){
    Motor.init();
    Expander.I2C.init(I2C_Motor,Output_Mode,All_Off);
    Expander.I2C.init(I2C_Schuss,Output_Mode,All_Off);
}

void SystemC::initC::Interface(){
    //RGB.init();
    #ifndef XCP_USB 
        Serial.begin(115200); 
    #endif

    #ifdef XCP_BL 
        xcpMaster_bl.Init();
    #endif 
    #ifdef XCP_USB 
        xcpMaster_usb.Init();
    #endif
    //Debug.begin();
    Expander.I2C.init(I2C_Button,Input_Mode);
    Expander.I2C.init(I2C_Dip_SW,Input_Mode);
}

void SystemC::initC::Sensors(){
    pinMode(Start_Port,INPUT);

    Expander.ADC.init(CS_IR);
    Expander.ADC.init(CS_LineA);
    Expander.ADC.init(CS_LineB);
    Expander.ADC.init(CS_LineC);
    Expander.ADC.init(CS_LineD);

    //INA.init();

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
        #ifdef XCP_BL 
            xcpMaster_bl.Event(0);
        #endif 
        #ifdef XCP_USB 
            xcpMaster_usb.Event(0);
        #endif
    }

    #ifdef XCP_BL 
      xcpMaster_bl.BackgroudTask();
    #endif 
    #ifdef XCP_USB 
      xcpMaster_usb.BackgroudTask();
    #endif
}

void SystemC::UpdateC::Sensors(){
    
    System.Start_Update();

    //Expander.ADC.read(CS_IR);
    Expander.ADC.read(CS_LineA);
    Expander.ADC.read(CS_LineB);
    Expander.ADC.read(CS_LineC);
    Expander.ADC.read(CS_LineD);

    //IR.read();

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