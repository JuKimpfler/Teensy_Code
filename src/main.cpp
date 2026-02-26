#include "Elementar.h"
#include "System.h"
#include "Cam.h"
#include "RGB.h"
#include "Mouse.h"
#include "US.h"
#include "Debug.h"
#include "BL.h"
#include "Game_Thread.h"

void setup() {
    SPI.begin();
    //Wire1.setClock(400000);
    Serial.begin(115200);
    UART_2.begin(115200);
    UART_Pixy.begin(115200);

    System.init();

    Robo_NR = "s";
}

void loop() { 
    Cycle_Timer = 0 ;

    System.Update.Sensors();
    System.Update.Interface();
 
    if(System.Start){
        Game.Run();
    }
    else{
        Robot.Stop();
    }


    if (System.Switches[0]){
        MainSpeed = HighSpeed;
    }
    else {
        MainSpeed = LowSpeed;
    }

    if(System.Button[0]){
        BNO055.Calibrate();
    }   
    if (System.Button[1]){
        Robot.Kicker.On();
        IR.Calib_Dist();
    }
    else{
        Robot.Kicker.Off();
    }
    if (System.Button[2]){
        Line.Calibrate(1);
    }
    if (System.Button[3]){
        Line.Calibrate(2);
        Line.Calibrate(3);
    }

    Motor.Enable = System.Switches[1];
    
    
    Cycle_P2++;
    Cycle_P3++;
    Cycletime = Cycle_Timer;

    Debug.Start();
    Debug.Plot("T",Mess_Time);
    Debug.Send();
}

