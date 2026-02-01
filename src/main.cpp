#include "Elementar.h"
#include "System.h"
#include "Cam.h"
#include "RGB.h"
#include "LineCalculations.h"
#include "Calculations.h"

String read = "";

void setup() {
    System.init.Sensors();
    System.init.Motors(); 
    System.init.Interface();
    Serial.begin(115200);
    UART_1.begin(115200);
}

void loop() {  
    Cycle_Timer = 0;

    System.Update.Sensors();
    System.Update.Interface();
    System.Update.Calculations();

    BNO055.showCal();
 
    if(System.Start){
        Game.Run();
        Serial.println("x: "+String(Cam.x)+" y: "+String(Cam.y)+" area: "+String(Cam.area));
    }
    else{
        //Robot.Stop();
        Game.Run();
        Serial.println(Cycletime);
    }

    /* Mouse Sensor Test
    Serial.print(">");
    Serial.print("lift: ");
    Serial.print(Mouse.lift);
    Serial.print(",motion: ");
    Serial.print(Mouse.movement);
    Serial.print(",x: ");
    Serial.print(Mouse.deltaX);
    Serial.print(",y: ");
    Serial.print(Mouse.deltaY);
    Serial.print(",diagonal: ");
    Serial.println(Mouse.delta_dist);
    */

    /* Distance IR Test
    Serial.print(">");
    Serial.print(",Distance: ");
    Serial.print(IR.Distance);
    Serial.print(",Distance_raw: ");
    Serial.print(IR.Distance_raw);
    Serial.print(",DistCal: ");
    Serial.print(IR.DistCal);
    Serial.print(",DistFaktor: ");
    Serial.println(IR.DistFaktor);
    delay(10);
    */

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

    //delay(1);
    
}

