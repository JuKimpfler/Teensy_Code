#include "System.h"
#include "ESC.h"
#include "servo.h"

Servo US_servo;

elapsedMillis debugTimer;
static constexpr uint32_t DEBUG_INTERVAL_MS = 20; // Serielle Ausgabe alle 100ms

void setup() {
    //US_servo.attach(Servo_Port);

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
    pinMode(Mouse_Port,INPUT);
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
    ESC.set(13);
    RGB.write(1,"G");  
    Serial.println("ON!");
    RGB.Apply();

    Cam.init(UART_2,115200);

    Cam.setSign(true);
}

void loop() { 
    Cycle_Timer = 0;
    MainSpeed = 30;

    if(System.Start){
        /*if(!Game.LineInterrupt()){
            if(LDR.Aktiv()){
                if(Cam.isValid()){Robot.Drive(Cam.give_Angle(),Cam.give_Angle(),30);Robot.Kicker.On();}
                else{Robot.Drive(0,0,30);}
            }
            else{
                int drive = U.Circel(((LUT.get_DriveAngle(U.Circel(Ball.Angle),Ball.Distance))));
                Robot.Drive(drive,0,20);
            }
        }*/
        if(Cam.isValid()){Robot.Drive(Cam.give_Angle()*1.3,-Cam.give_Angle()/4,20);}
        else{Robot.Drive(0,0,20);}
        //if(Cam.isValid()){Robot.Turn(Cam.give_Angle());}
        //else{Robot.Turn(0);}
        //Robot.Turn(0);
        //Robot.Drive(45,0,10);
    } 
    else{
        Game.Stop();
        //BC.sendTelemetryFloat("test",0.321);
    

        if(System.Button[0]){BNO055.Calibrate();} // BNO055 set to 0

        if (System.Button[1]){Robot.Kicker.On();} // Kicker test

        if (System.Button[2]){IR.Calib_Dist();} 
        else{}

    }

    Cam.Update();

    if (debugTimer >= DEBUG_INTERVAL_MS ) {
        debugTimer = 0;

        //Serial.print(PFU.giveX());
        //Serial.print(",");
        //Serial.println(PFU.giveY());

        Serial.print("> ");
        if (Cam.isValid()) {
            float a = Cam.give_Angle();
            float a_r = Cam.give_Angle_Cam();
            float h = Cam.give_BlobH();
            Serial.println(" Angle: "+String(a)+" , Angle_rela: "+String(a_r)+" , höhe: "+String(h));
        }
        Serial.println(" LDR: "+String(Line.Summe)+" , LDRa: "+String(Cycletime));
        System.Update.Interface();
    }

    System.Update.Calculations();
    System.Update.Sensors();

    Robot.Kicker.Update_End();
    Cycletime=Cycle_Timer;
    //Serial.println("Cycle: "+String(Cycletime));
}