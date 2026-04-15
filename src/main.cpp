#include "System.h"

void setup() {
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);

    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);
    delay(1000);
    Expander.I2C.read(I2C_ITF_Main);
    Color_ID = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_CID);

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
}

void loop() { 
    if(System.Start){
        if(System.Button[0]){
            Ball.Angle = 20;
        }
        else{
            Ball.Angle = -170;
        }
        if(System.Button[1]){
            Ball.Distance = 2;
        }
        else{
            Ball.Distance = 180;
        }

        //Game.Run();
        int drive = LUT.get_DriveAngle(Ball.Angle,Ball.Distance*0.9);
        Robot.Drive(drive,0,MainSpeed);

        Debug.Start();
        Debug.Plot("In",Ball.Angle);
        Debug.Plot("Dist",Ball.Distance);
        Debug.Plot("Out",drive);
        Debug.Send();
        delay(20);
    }
    else{
        Game.Stop();
    }
    System.Update.Calculations();
    System.Update.Sensors();
    System.Update.Interface();
}