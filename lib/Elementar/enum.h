#pragma once
#include "Arduino.h"

#ifndef enums_h
#define enums_h

enum Motor_Port { // Portbelegung direct an Teensy
    VR_Motor = 1,
    VL_Motor = 2,
    HR_Motor = 3,
    HL_Motor = 4, 

    VR_Speed_Port = 4,
    VL_Speed_Port = 5,
    HR_Speed_Port = 6,
    HL_Speed_Port = 9,
    DR_Speed_Port = 33,

    VR_PortA = 0,
    VR_PortB = 1,
    VL_PortA = 2,
    VL_PortB = 3,
    HR_PortA = 7,
    HR_PortB = 8,
    HL_PortA = 26,
    HL_PortB = 32,
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
    Kicker_Port = 18,
};

enum I2C_Befehle {
    Output_Reg = 0x01, // I2c Expander select Output Register to write to the expander
    Input_Reg = 0x00, // I2c Expander select Input Register to read the expander
    Config_Reg = 0x03, // I2c Expander configurate in and Outputs on the expander (0 -> Output / 1 -> Input)

    Input_Mode = 0xFF,
    ITF1_config = 0xFF,
    ITF2_config = 0b11000000,
    Ex_config = 0b11111110,
    Output_Mode = 0x00,
 
    All_Off = 0x00,
    All_On = 0xFF,
};

enum I2C_Port {
    I2C_Ex = 0x20, // Buttons und Switches Main Platine
    I2C_INA219_Main = 0x40, // Batterie Voltage
    I2C_INA219_DR = 0x41, // Dribbler Voltage
    I2C_ITF_1 = 0x26,
    I2C_ITF_2 = 0x27
};

enum ADC_Port {
    CS_LineA = 19,
    CS_LineB = 31,
    CS_LineC = 30,
    CS_LineD = 21,
    CS_Line_OUT = 22,
};

enum I2C_Portexpander {
    BTN1_Ex = 0,
    BTN2_Ex = 1,
    BTN3_Ex = 2,
    BTN4_Ex = 3,
    SW1_Ex = 4,
    SW2_Ex = 5,
    SW3_Ex = 6,
    LED_Ex = 7,

    SW1_ITF1 = 0,
    SW2_ITF1 = 1,
    SW3_ITF1 = 2,
    SW4_ITF1 = 3,
    SW5_ITF1 = 4,
    Start_ITF1 = 5,
    BTN1_ITF1 = 6,
    BTN2_ITF1 = 7,

    BTN1_ITF2 = 0,
    BTN2_ITF2 = 1,
    LED1_ITF2 = 2,
    LED2_ITF2 = 3,
    LED3_ITF2 = 4,
    LED4_ITF2 = 5,
    LED5_ITF2 = 6,
    LED6_ITF2 = 7
};

//Seriel Port
#define UART_Cam Serial3
#define UART_1 Serial6
#define UART_2 Serial7

#endif