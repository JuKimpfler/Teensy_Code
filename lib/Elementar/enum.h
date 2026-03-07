#pragma once
#include "Arduino.h"

#ifndef enums_h
#define enums_h

// schwarz
inline int VR_Motors = 4; //
inline int VL_Motors = 3; //
inline int HR_Motors = 2; //
inline int HL_Motors = 1; //

// weis
inline int VR_Motor = 4; //
inline int VL_Motor = 3; //
inline int HR_Motor = 1; //
inline int HL_Motor = 2; //

enum Motor_Port { // Portbelegung Direct an Teensy
    VR_Speed_Port = 9, //+
    VL_Speed_Port = 6, //+
    HR_Speed_Port = 4, //+
    HL_Speed_Port = 5, //+

    VR_ExpanderA = 26, //+
    VR_ExpanderB = 32, //+
    VL_ExpanderA = 8, //+
    VL_ExpanderB = 7, //+
    HR_ExpanderA = 0, //+
    HR_ExpanderB = 1, //+
    HL_ExpanderA = 2, //+
    HL_ExpanderB = 3, //+
};

enum US_Ports {
    US_Leftw =  0x71,
    US_Rightw = 0x72,
    US_Backw = 0x70,
    
    US_Backs = 0x74,
    US_Lefts =  0x73,
    US_Rights = 0x71
};

enum Pinout {
    LDR_Port = 23,
    Start_Port = 10,
    RCJ_Port = 7,
    BT_State_Port = 31,
    Schuss_Port = 30,
    Kicker_Port = 18
};

enum I2C_Befehle {
    Output_Reg = 0x01, // I2c Expander select Output Register to write to the expander
    Input_Reg = 0x00, // I2c Expander select Input Register to read the expander
    Config_Reg = 0x03, // I2c Expander configurate in and Outputs on the expander (0 -> Output / 1 -> Input)

    Input_Mode = 0xFF,
    Output_Mode = 0x00,

    All_Off = 0x00,
    All_On = 0xFF,
};

enum I2C_Port {
    I2C_ITF_Main = 0x20, // Buttons und Switches Main Platine
    I2C_Motor = 0x21, // Motoren
    I2C_ITF_Ex  = 0x22, // Interface_Platine
    I2C_INA219 = 0x40 // Batterie Voltage
};

enum I2C_Pin {
    ITF_Main_LED = 7,
    ITF_Main_SW0 = 4,
    ITF_Main_SW1 = 5,
    ITF_Main_SW2 = 6,
    ITF_Main_BT0 = 0,
    ITF_Main_BT1 = 1,
    ITF_Main_BT2 = 2,
    ITF_Main_BT3 = 3, 
};

enum ADC_Port {
    CS_LineA = 19,
    CS_LineB = 31,
    CS_LineC = 30,
    CS_LineD = 21,
    CS_LineVW = 22
};


#define I2C_BUS Wire1        // IRL-Ring on Wire1
#define I2C_SPEED 1000000    // 1 MHz
#define I2C_SPEED_US 100000  // 100 kHz for US sensor queries


//Seriel Port
#define UART_Pixy Serial3
#define UART_1 Serial6
#define UART_2 Serial7

#endif