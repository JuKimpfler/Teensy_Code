#include "Elementar.h"
#include "System.h"
#include "Cam.h"
#include "RGB.h"


void setup() {
    System.init.Sensors() ;
    System.init.Motors() ;
    System.init.Interface()   ;
}

void loop() { 
    Cycle_Timer = 0 ;

    System.Update.Sensors();
    System.Update.Interface();
    System.Update.Calculations();

    BNO055.showCal();
 
    if(System.Start){
        Game.Run();

        //Robot.Turn(0);

        //Robot.Drive(0,0,20); 

        //Serial.print("Angle: "); Serial.println(BNO055.give_TiltX());

        //Serial.println(PID.Out);

        

        //Serial.print("Calibrated data: ");
        //for (int i=0; i<16; i++)
        //{
        //    Serial.print(i+1); Serial.print(": ");
        //    Serial.print(IR.IR_Values[i]); Serial.print("  ");
        //}

        //Robot.Drive(BallSearchCalculations.OutAngle,0,MainSpeed);
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
        //UART_Debug.print("Ball Drive: "); UART_Debug.println(BallSearchCalculations.OutAngle);
        //Serial.println("1");

    }

    if(System.Button[0]){
        BNO055.Calibrate();
    }   

    //if(System.Button[1]){
    //    Robot.Kicker.Once();
    //    RGB.write(1,"G");
    //} 
    //else{
    //    Robot.Kicker.Off();
    //    RGB.write(1,"OFF");
    //}

    if (System.Switches[0]){
        MainSpeed = HighSpeed;
    }
    else {
        MainSpeed = LowSpeed;
    }
    
    Cycletime = Cycle_Timer;
    
}

