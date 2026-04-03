#include "Elementar.h"
#include "System.h"
#include "Lowlevel.h"

void setup() {
    SPI.begin();
    Serial.begin(115200);
    UART_2.begin(115200);
    UART_Pixy.begin(115200);
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);

    pinMode(Start_Port,INPUT);
    pinMode(Kicker_Port, OUTPUT);
}

void loop() { 
    
}