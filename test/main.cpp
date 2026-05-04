#include "System.h"
#include "ESC.h"
#include "servo.h"

Servo US_servo;

elapsedMillis debugTimer;
static constexpr uint32_t DEBUG_INTERVAL_MS = 20; // Serielle Ausgabe alle 100ms

void setup() {
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);
    
    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);
    delay(500);
    Expander.I2C.read(I2C_ITF_Main);
    Color_ID = Expander.I2C.give(I2C_ITF_Main,ITF_Main_CID);

    SPI.begin();
    Serial.begin(115200);
    UART_2.begin(115200);
    UART_Pixy.begin(115200);

    pinMode(Start_Port,INPUT);
    pinMode(Kicker_Port, OUTPUT);
    pinMode(RCJ_Port,INPUT);
    System.begin(Color_ID);

    RGB.write(0,"Off");
    RGB.write(1,"Off");
    RGB.write(2,"Off");

    RGB.write(1,"R");  
    Serial.println("push button 3");
    RGB.Apply();
    ESC.init(33);
    delay(1000);
    while(!System.Button[2]){System.Update.Interface();Serial.println("waiting on power up");}
    delay(100);
    ESC.init_Power();
    ESC.set(10);
    RGB.write(1,"G");  
    Serial.println("ON!");
    RGB.Apply();
    delay(5000);
    ESC.stop();

    Cam.init(UART_2,115200);

    Cam.setSign(true);
}

void loop() { 
    Cycle_Timer = 0;

    if((digitalRead(RCJ_Port)) || System.Start){
        Game.Run();
    } 
    else{
        Game.Stop();
        ESC.stop();    

        if(System.Button[0]){BNO055.Calibrate();} // BNO055 set to 0

        if (System.Button[1]){Robot.Kicker.On();} // Kicker test

        if (System.Button[2]){IR.Calib_Dist();} 
        else{}

        if (System.Switches[1]){IR.Calib_Offset();} 
        else{}

    }

    if (debugTimer >= DEBUG_INTERVAL_MS ) {
        debugTimer = 0;
        Serial.print("> ");
        BC.sendTelemetryFloat("IR_Angle",Ball.Angle);
        Serial.println(" Anglw: "+String(analogRead(LDR_Port))+" , Dist: "+String(Ball.Distance));
        System.Update.Interface();
    }

    System.Update.Calculations();
    System.Update.Sensors();

    Robot.Kicker.Update_End();
    Cycletime=Cycle_Timer;
    //Serial.println("Cycle: "+String(Cycletime));
}