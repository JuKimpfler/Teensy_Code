#pragma once
#include "Arduino.h"

#ifndef enums_h
#define enums_h

enum Motor_Port { // Portbelegung Direct an Teensy
    VR_Motor = 1, //4
    VL_Motor = 2, //9
    HR_Motor = 3, //5
    HL_Motor = 4, //6

    VR_Speed_Port = 4,
    VL_Speed_Port = 9,
    HR_Speed_Port = 5,
    HL_Speed_Port = 6,
    DR_Speed_Port = 2,

    VR_ExpanderA = 7,
    VR_ExpanderB = 6,
    VL_ExpanderA = 1,
    VL_ExpanderB = 0,
    HR_ExpanderA = 5,
    HR_ExpanderB = 4,
    HL_ExpanderA = 3,
    HL_ExpanderB = 2,
};

enum Ultrasonic_Address{
    US_Front = 0x70,
    US_Left = 0x71,
    US_Back = 0x72,
    US_Right = 0x73,
};

enum Pinout {
    LDR_Port = 23,
    Start_Port = 10,
    RCJ_Port = 7,
    BT_State_Port = 31,
    Schuss_Port = 30,
    Kicker_Port = 3
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
    ITF_Main_LED = 2,
    ITF_Main_Dip0 = 7,
    ITF_Main_Dip1 = 6,
    ITF_Main_Dip2 = 5,
    ITF_Main_Dip3 = 4,
    ITF_Main_SW = 3,
    ITF_Main_BT0 = 1,
    ITF_Main_BT1 = 0,
    
    
};

enum ADC_Port {
    CS_LineA = 32,
    CS_LineB = 12,
    CS_LineC = 13,
    CS_LineD = 22,
    CS_LineOUT = 33,

    CS_IR = 11,
    IRQ_IR = 0,
};


//Seriel Port
#define UART_Debug Serial6
#define UART_BT Serial5
#define Uart_Cam1 Serial3
#define UART_Cam2 Serial7

#endif