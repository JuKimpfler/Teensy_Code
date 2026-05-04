#include "System.h"
#include "Defender.h"

elapsedMillis debugTimer;
static constexpr uint32_t DEBUG_INTERVAL_MS = 20; // Serielle Ausgabe alle 100ms

void setup() {
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);
    
    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);
    delay(500);
    Expander.I2C.read(I2C_ITF_Main);
    Color_ID = Expander.I2C.give(I2C_ITF_Main,ITF_Main_CID);
    ESC.Enable = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW0);

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

    if(ESC.Enable){
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
    }
    else{
        RGB.write(1,"G");  
        Serial.println("ON!");
        RGB.Apply();
    }
    

    Cam.init(UART_2,115200);

    Cam.setSign(true);

    Defender.set_State(true);
}

void loop() { 
    Cycle_Timer = 0 ;

    BC.process();
    
    if(System.Start || BC.start || digitalRead(RCJ_Port)){ 
        if (BC.mode1) {
            //Game.Run();
            Defender.Update();
        }
        else if (BC.mode2) {
            if (BC.controlActive) {Robot.Drive(BC.angle,0,BC.speed);}else{Game.Stop();}
        }
        else if (BC.mode3) {Robot.Turn(0);}
        else if (BC.mode4) {Robot.Turn(Cam.give_Angle());}

        else if (!BC.mode5) {Defender.Update();} // Ohne BC modus
    }
    else{
        Game.Stop();
        ESC.stop();

        if(System.Button[0] || BC.Bt1 ){BNO055.Calibrate();} // BNO055 set to 0

        if (System.Button[1] || BC.Bt2 ){Robot.Kicker.On();} // Kicker test

        if (System.Button[2] || BC.Bt3 ){IR.Calib_Dist();} 
        else{}

        if (System.Switches[1] || BC.Sw1){IR.Calib_Offset();} 
        else{}
    }

    if(BC.mode5){
        if (BC.calIrMax)// nichts
        {}
        if (BC.calIrMax)// nichts
        {}
        if (BC.calIrMax)// nichts
        {}
        if (BC.calIrMax)// nichts
        {}
        if (BC.calIrMax) // 
        {}
    }

    //if (BC.Sw3) {
        if (debugTimer >= DEBUG_INTERVAL_MS ) {
            debugTimer = 0;

            //Serial.print("> ");
           // Serial.println(" St: "+String(BallCalc.lastD)+" , D: "+String(BallCalc.diffD)+" , A: "+String(Ball.Distance));

           // BC.sendTelemetryFloat("IR_Angle",Ball.Angle);
           // BC.sendTelemetryFloat("Cam_Angle",Cam.give_Angle());            
        }
    //}

    
    System.Update.Interface();

    System.Update.Calculations();
    System.Update.Sensors();

    Robot.Kicker.Update_End();
    Cycletime=Cycle_Timer;
    //Serial.println("Cycle: "+String(Cycletime));
}

