#include "Elementar.h"
#include "System.h"
#include "Cam.h"
#include "RGB.h"
#include "LineCalculations.h"
#include "Debug.h"



void setup() {
    SPI.begin();
    System.init.Sensors();
    System.init.Motors(); 
    System.init.Interface();
    Serial.begin(115200);
    UART_2.begin(115200);
    UART_Pixy.begin(115200);
}

void loop() {  
    Debug.Start();
    Debug.Plot("Time",Cycletime);

    Cycle_Timer = 0;
    //Mouse.read();
    System.Update.Sensors();
    System.Update.Interface();
    System.Update.Calculations();

    BNO055.showCal();
 
    if(Robot.Start && Mouse.lift == true){
        Game.Run();}
    else{
        Robot.Stop();
        //Game.Run();
    }

    //Mouse Sensor Test
    Debug.Plot("line",Line.Summe+Line.Summe_VW);
    Debug.Plot("lift",Mouse.lift);
    Debug.Plot("move",Mouse.movement);
    Debug.Plot("x",Mouse.deltaX);
    Debug.Plot("y",Mouse.deltaY);
    Debug.Plot("xpos",Mouse.xPos);
    Debug.Plot("ypos",Mouse.yPos);
    Debug.Plot("speed",Mouse.delta_dist);
    Debug.Send();
        

    if(Robot.Button[0]){
        BNO055.Calibrate();
    }   
    if(Robot.Button[1]){
        IR.Calib_Offset();
    }   
    if(Robot.Button[2]){
        IR.DistCal = IR.Distance_raw;
    }   
    if(Robot.Button[3]){
        IR.Calib_Dist();
    }   

    if (Robot.Switches[0]){
        MainSpeed = HighSpeed;
    }
    else {
        MainSpeed = LowSpeed;
    }
    
    Cycletime = Cycle_Timer;    
}

