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

    Robo_NR = "w"; // w oder s
    if(Robo_NR == "s"){
        IR.DistCal = 100;
        VR_Motor = VR_Motors; //
        VL_Motor = VL_Motors; //
        HR_Motor = HR_Motors; //
        HL_Motor = HL_Motors; //
        LDR_Schwelle = LDR_Schwelle_s ;
    }
    System.init();
}

void loop() { 
    Cycle_Timer = 0 ;

    System.Update.Sensors();
    System.Update.Interface();
 
    if(System.Start){ 
        Game.Run();
        //Robot.Turn(0);
        //Robot.Drive(-90,0,20);
        //Motor.On(100,VR_Motor);
        //Motor.On(100,VR_Motor);
        //Motor.On(100,HL_Motor);
        //Motor.On(100,HR_Motor);
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
    }
    else{
        Robot.Kicker.Off();
    }

    if (System.Switches[1]){
        if (System.Button[2]){
            IR.DistCal = Ball.Distance_raw2;
        }
        if (System.Button[3]){
            Line.Calibrate(2);
            Line.Calibrate(3);
        }
    }
    else{
        if (System.Button[2]){
            IR.DistCal = Ball.Distance_raw2;
        }
        if (System.Button[3]){
            
        }
    }
    
    if(Robo_NR=="w"){
        //Motor.Enable = System.Switches[1];
    }
    else{
        //Motor.Enable = true;
    }
    Motor.Enable = true;
    
    
    Cycle_P2++;
    Cycle_P3++;
    Cycletime = Cycle_Timer;

    //Debug.Start();
    //Debug.Plot("cycle",Cycletime);
    //Debug.Send();
}

