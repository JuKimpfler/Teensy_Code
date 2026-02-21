#include "enum_V5.h"
#include "Motor.h"


void setup() {
    Motor.init();
    pinMode(Start_Port,INPUT);
}

void loop() { 
    if (digitalRead(Start_Port)==1){Motor.On(100,VR_Motor);}else{Motor.Off(VR_Motor);}
}

