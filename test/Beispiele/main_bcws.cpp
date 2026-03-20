#include "Arduino.h"
#include "BotConnect.h"
#include "Elementar.h"
#include "Robot.h"
#include "Lowlevel.h"

void onMode(uint8_t modeId) {
    Serial.printf("Mode selected: %d\n", modeId);
    // Set your robot mode variable here
}

void onCalibrate(const char *calCmd) {
    Serial.printf("Calibrate: %s\n", calCmd);
    // Handle calibration command
}

void onControl(int16_t speed, int16_t angle, 
               uint8_t switches, uint8_t buttons, uint8_t start) {
    Serial.printf("Ctrl: V=%d A=%d SW=0x%02X BTN=0x%02X START=%d\n",
                  speed, angle, switches, buttons, start);
    Robot.Drive(angle,0);
    Serial.println("angle"+String(-angle));
    // Drive your motors here
}

void setup() {
    Serial.begin(115200);
    Serial3.begin(115200);
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);

    pinMode(Start_Port,INPUT);
    pinMode(Kicker_Port, OUTPUT);

    Motor.init();
    BNO055.init(); 
    
    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);

    // SAT_ID=1 means this Teensy is attached to ESP_Satellite with SAT_ID=1
    BC.begin(Serial3, 1);
    BC.onMode(onMode);
    BC.onCalibrate(onCalibrate);
    BC.onControl(onControl);
    BC.setDebugEnabled(false);

    Serial.println("BotConnect ready");
}

void loop() {
    //Robot.Drive(0,0);
    BC.process();

    BNO055.read();
    PID.Calculate(); // 1 micro

    // Example: send telemetry every 50 ms
    static uint32_t lastTelem = 0;
    if (millis() - lastTelem >= 50) {
        lastTelem = millis();
        BC.sendTelemetryFloat("BallAngle", 12.5f);
        BC.sendTelemetryFloat("BallDist",  120.0f);
        BC.sendTelemetryBool("Start",      true);
        BC.sendTelemetryInt("Mode",        1);
    }


}