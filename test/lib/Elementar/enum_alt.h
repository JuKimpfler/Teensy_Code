#pragma once
#include "Arduino.h"

#ifndef enums_h
#define enums_h

enum Motor_Port { // Portbelegung
    VR_Motor = 0, //76
    VL_Motor = 3, //10
    HR_Motor = 1, //54
    HL_Motor = 2, //32
    Dribbler_Motor = 4,

    VR_Speed_Port = 4,
    VL_Speed_Port =  9,
    HR_Speed_Port = 5,
    HL_Speed_Port = 6,
    DR_Speed_Port = 33,

    VR_ExpanderA = 7,
    VR_ExpanderB = 6,
    VL_ExpanderA = 1,
    VL_ExpanderB = 0,
    HR_ExpanderA = 5,
    HR_ExpanderB = 4,
    HL_ExpanderA = 3,
    HL_ExpanderB = 2,
    DR_ExpanderA = 0,
    DR_ExpanderB = 0,
};

enum Ultrasonic_Address{
    US_Front = 0x70,
    US_Left = 0x71,
    US_Back = 0x72,
    US_Right = 0x73,
};

enum Pinout {
    LDR_Port = 23,
    Batterie_check = 25,
    Start_Port = 10
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
    I2C_Button = 0x20, // Buttons und Switches Main Platine
    I2C_Dip_SW = 0x21, // Switches und 2x? Main Platine
    I2C_Motor  = 0x22, // Motoren Line Platine (ACHTUNG !!!: Wire1)
    I2C_Schuss = 0x23, // Schuss und 3x? Line PLatine (ACHTUNG !!!: Wire1)
    I2C_INA219 = 0x40 // Batterie Voltage
};

enum ADC_Port {
    CS_LineA = 2,
    CS_LineB = 3,
    CS_LineC = 7,
    CS_LineD = 8,

    CS_IR = 0
};

//Bitmasken
#define LINE_EN_OFF ( (uint8_t)(1 << 2)) 
#define LINE_EN_ON  (~(uint8_t)(1 << 2))

//Seriel Port
#define Serial Serial7
#define UART_Cam Serial3

#endif