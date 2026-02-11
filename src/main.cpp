#include "Elementar.h"
#include "System.h"
#include "Cam.h"
#include "RGB.h"
#include "LineCalculations.h"
#include "Calculations.h"

String read = "";

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
    Serial.print(">");
    Serial.print("CT: ");
    Serial.print(Cycletime);

    Cycle_Timer = 0;
    //Mouse.read();
    System.Update.Sensors();
    System.Update.Interface();
    System.Update.Calculations();

    BNO055.showCal();
 
    if(System.Start && Mouse.lift == false){
        Game.Run();
        //Robot.Turn(0);
        //Serial.println("x: "+String(Goal.X)+" y: "+String(Goal.Y)+" Angle: "+String(Goal.Angle)+" Area: "+String(Goal.Area));
        //Serial.println("l: "+String(US.Distance_raw[1])+" r: "+String(US.Distance_raw[0])+" timer1: "+String(US_Timer1)+" d: "+String(US.Distance));
    }
    else{
        Robot.Stop();
        //Game.Run();
        //Serial.println(Cycletime);
    }

    //Mouse Sensor Test
    Serial.print(" ,lift: ");
    Serial.print(Mouse.lift);
    Serial.print(",motion: ");
    Serial.print(Mouse.movement);
    Serial.print(",x: ");
    Serial.print(Mouse.deltaX);
    Serial.print(",y: ");
    Serial.print(Mouse.deltaY);
    Serial.print(",diagonal: ");
    Serial.println(Mouse.delta_dist);

    /*Serial.print(",A: ");
    Serial.print(Ball.Angle);
    Serial.print(",z: ");
    Serial.print(BNO055.give_TiltZ());
    Serial.print(",pid: ");
    Serial.println(PID.Out);*/

    /*//Distance IR Test
    Serial.print(",Distance: ");
    Serial.print(Ball.Distance);
    Serial.print(",DistCal: ");
    Serial.print(IR.DistCal);
    Serial.print(",DistFaktor: ");
    Serial.println(IR.DistFaktor);
    delay(10);*/
    

    if(System.Button[0]){
        BNO055.Calibrate();
    }   
    if(System.Button[1]){
        IR.Calib_Offset();
    }   
    if(System.Button[2]){
        IR.DistCal = IR.Distance_raw;
    }   
    if(System.Button[3]){
        IR.Calib_Dist();
    }   

    if (System.Switches[0]){
        MainSpeed = HighSpeed;
    }
    else {
        MainSpeed = LowSpeed;
    }
    
    Cycletime = Cycle_Timer;

    delay(10);
    
}

