#include "Elementar.h"
#include "System.h"
#include "Cam.h"
#include "RGB.h"
#include "Mouse.h"
#include "US.h"
#include "Debug.h"
#include "Game_Thread.h"
#include "BotConnect.h"

void setup() {
    SPI.begin();
    Serial.begin(115200);
    UART_2.begin(115200);
    UART_Pixy.begin(115200);

    #ifdef Robo_s 
        System.Calibrate("s");
        VR_Motor = VR_Motors; 
        VL_Motor = VL_Motors; 
        HR_Motor = HR_Motors; 
        HL_Motor = HL_Motors;
        Motor.Enable = true;
        BC.begin(UART_Pixy, 1);
    #endif
    #ifdef Robo_w
        System.Calibrate("w");
        Motor.Enable = true;
        BC.begin(UART_Pixy, 2);
    #endif

    System.init();

    delay(1000);

    BC.onP2P([](const char *msg) {
        Serial.printf("P2P from peer: %s\n", msg);
    });
    
    #ifdef Com_Debug
    BC.setDebugEnabled(true);
    #endif
}

void loop() { 
    Robot.Kicker.Off();
    Cycle_Timer = 0 ;

    System.Update.Sensors();
    System.Update.Interface();

    BC.process();
    
    #ifndef PID_Calib
    if(System.Start || BC.start){ 
        // ── Mode ─────────────────────────────────────────────────
        // Exactly one modeX variable is true at a time (after first mode command).
        if (BC.mode1) {
            Game.Run();
        }
        if (BC.mode2) {
            // ── Control ───────────────────────────────────────────────
            // controlActive is true while commands were received within the last 500 ms.
            if (BC.controlActive) {Robot.Drive(BC.angle,0,BC.speed);}else{Robot.Stop();}
        }
        if (BC.mode3) {Robot.Turn(0,40);}
        if (BC.mode4) {Robot.Stop();}
        if (BC.mode5) {Robot.Stop();}

        // ── Calibration ──────────────────────────────────────────
        // The last received calibration command's variable is true.
        if (BC.calIrMax)   { /* run IR max calibration */ }
        if (BC.calIrMin)   { /* run IR min calibration */ }
        if (BC.calLineMax) { /* run line max calibration */ }
        if (BC.calLineMin) { /* run line min calibration */ }
        if (BC.calBno)     { /* run BNO sensor calibration */ }

        // ── Telemetry ─────────────────────────────────────────────
        static uint32_t lastTelem = 0;
        if (millis() - lastTelem >= 50) {
            lastTelem = millis();
            BC.sendTelemetryFloat("BallAngle", Ball.Angle);
            BC.sendTelemetryFloat("BallDist",  Ball.Distance);
            BC.sendTelemetryBool("Start", System.Start);
        }
    }
    else{
        Robot.Stop();
    }
    #endif

    // Interface

    // Switches
    if (System.Switches[0]){MainSpeed = HighSpeed;}
    else {MainSpeed = LowSpeed;}

    Goal_Turn = System.Switches[1];

    if(System.Button[0]){BNO055.Calibrate();Line.Calibrate(1);} // BNO055 set to 0

    if (System.Button[1]){Robot.Kicker.On();} // Kicker test
 
    if (System.Button[2]){IR.Calib_Dist();} 
    
    Cycle_P2++;
    Cycle_P3++;
    Cycletime = Cycle_Timer;
    Robot.Kicker.Update();
}

