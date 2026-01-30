#include "Elementar.h"
#include "System.h"
#include "Cam.h"
#include "RGB.h"
#include "LineCalculations.h"
#include "Calculations.h"

String read = "";

void setup() {
    System.init.Sensors() ;
    System.init.Motors() ;
    System.init.Interface()   ;
    Mouse.init();
}

void loop() { 
    Mouse.read();

    System.Update.Sensors();
    System.Update.Interface();
    System.Update.Calculations();

    BNO055.showCal();
 
    if(System.Start){
        //Game.Run();
        //LineCalc.Calc();

        //Taktics.BallSearch();

        // Robot.Turn(0);

        //Robot.Drive(0,0,20); 

        Serial.print("Angle: "); Serial.println(BallCalc.DriveAngle);

        //Serial.println(PID.Out);

        //Serial.println("Hi:"+String(LineCalc.DriveAngle));
        //Motor.On()

        //Serial.print("Calibrated data: ");
        //for (int i=0; i<16; i++)
        //{
        //    Serial.print(i+1); Serial.print(": ");
        //    Serial.print(IR.IR_Values[i]); Serial.print("  ");
        //}

        //Robot.Drive(BallCalc.DriveAngle,0,MainSpeed);
        //Line.read();
        //Serial.println("Hallo1234");
    }
    else{
        Robot.Stop();
        //RGB.write(0,"OFF");
        //RGB.write(1,"OFF");
        //RGB.write(2,"OFF");
        //UART_Debug.print("Ball angle: "); UART_Debug.println(IR.Angle);
        //UART_Debug.print("Ball distance: "); UART_Debug.println(IR.Distance);
        //UART_Debug.print("Ball Drive: "); UART_Debug.println(BallCalc.DriveAngle);
        //Serial.println("1");

        //if(UART_1.available()){
        //    UART_Debug.println(UART_1.readString());
        //}
        //else{UART_Debug.println("Hi");UART_Debug.println("Hi");}
    }
    /*
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
    Serial.println(Mouse.delta_dist);*/

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
    /*if(Serial.available()){
        Serial.print("  Empfang   ");
        read=Serial.readString();
        if(read!="Hi12"){
            digitalWrite(LED_BUILTIN,LOW);
            Serial.print(read);
        }
    }

    if(read=="Hi12"){
        digitalWrite(LED_BUILTIN,HIGH);
    }*/

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
    
}

