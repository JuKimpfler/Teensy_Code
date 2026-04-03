#include <Arduino.h>
#include "Param.h"
#include "BNO055.h"
#include "PidCalculations.h"
#include "Robot.h"

void printPID() {
    Serial.print("Kp: "); Serial.print(Kp);
    Serial.print(" | Ki: "); Serial.print(Ki);
    Serial.print(" | Kd: "); Serial.print(Kd); 
    Serial.print(" | PID_Mult: "); Serial.println(PID_Mult);
}

void setup() {
    BNO055.init();
    Motor.init();
    Serial.begin(115200);
    printPID();
    Serial.println("PID Parameter Tool bereit.");
    Serial.println("Befehl: kp <wert>, ki <wert>, kd <wert>, mult <wert>");
}

void loop() {
    BNO055.read();
    BNO055.showCal();
    if(digitalRead(Start_Port)==false){
        BNO055.Calibrate();
    }
    PID.Calculate();
    Robot.Turn(0);
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        if (input.startsWith("kp ")) {
            Kp = input.substring(3).toFloat();
            Serial.print("Neues Kp: "); Serial.println(Kp);
        } else if (input.startsWith("ki ")) {
            Ki = input.substring(3).toFloat();
            Serial.print("Neues Ki: "); Serial.println(Ki);
        } else if (input.startsWith("kd ")) {
            Kd = input.substring(3).toFloat();
            Serial.print("Neues Kd: "); Serial.println(Kd);
        } else if (input.startsWith("mult ")) {
            PID_Mult = input.substring(5).toFloat();
            Serial.print("Neues PID_Mult: "); Serial.println(PID_Mult);
        } else if (input == "show") {
            printPID();
        } else {
            Serial.println("Unbekannter Befehl. Nutze: kp <wert>, ki <wert>, kd <wert>, mult <wert>, show");
        }
    }
}
