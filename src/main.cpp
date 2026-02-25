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
    Serial.begin(115200);
    UART_2.begin(115200);
    UART_Pixy.begin(115200);
    System.init.Sensors() ;
    System.init.Motors() ;
    System.init.Interface()   ;

    Mouse.init();
    //BL.init();
    //US.init();
}

void loop() { 
    Cycle_Timer = 0 ;

    System.Update.Sensors();
    System.Update.Interface();

    //BNO055.showCal();

    //Mouse.read();
    //US.read();
    //BL.doRolle();
    Robot.Kicker.Update();
 
    if(System.Start){
        //Robot.Kicker.On();
        Game.Run();

        //Taktics.BallSearch();

        //Robot.Drive(0,0,30); 

        //Robot.Drive(Ball.Angle,0,20); 
        //Robot.Drive(0,0,20);
        
        //Serial.println(PID.Out);

        //Serial.println("Hi:"+String(LineCalc.DriveAngle));
        //Motor.On()

        //Serial.print("Calibrated data: ");
        //for (int i=0; i<16; i++)
        //{
        //    Serial.print(i+1); Serial.print(": ");
        //    Serial.print(IR.IR_Values[i]); Serial.print("  ");
        //}

        //Robot.Drive(BallCalc.OutAngle,0,MainSpeed);
        //Line.read();
        //Serial.println("Hallo1234");
    }
    else{
        Robot.Stop();
        Robot.Kicker.Off();
        //RGB.write(0,"OFF");
        //RGB.write(1,"OFF");
        //RGB.write(2,"OFF");
        //Serial.print("Ball angle: "); Serial.println(Ball.Angle);
        //Serial.print("Ball distance: "); Serial.println(Ball.Distance);
        //Serial.print("Ball Drive: "); Serial.println(BallCalc.OutAngle);
        //Serial.println("1");

        //if(UART_1.available()){
        //    Serial.println(UART_1.readString());
        //}
        //else{Serial.println("Hi");Serial.println("Hi");}
    }

    if(System.Button[0]){
        BNO055.Calibrate();
    }   
    if (System.Switches[0]){
        MainSpeed = HighSpeed;
    }
    else {
        MainSpeed = LowSpeed;
    }
    if (System.Button[1]){
        Robot.Kicker.Once();
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
    //delay(10);
}

