#include "Elementar.h"
#include "System.h"
#include "Cam.h"
#include "RGB.h"
#include "Mouse.h"
#include "US.h"
#include "Debug.h"


void setup() {
    SPI.begin();
    Serial.begin(115200);
    UART_2.begin(115200);
    UART_Pixy.begin(115200);
    System.init.Sensors() ;
    System.init.Motors() ;
    System.init.Interface()   ;

    Mouse.init();

    US.init();
}

void loop() { 
    Cycle_Timer = 0 ;

    System.Update.Sensors();
    System.Update.Interface();
    System.Update.Calculations();

    BNO055.showCal();

    Mouse.read();
    US.read();
    Debug.Start();
 
    if(System.Start){
        Game.Run();

        //Taktics.BallSearch();

        // Robot.Turn(0);

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

    Debug.Plot("L",BNO055.TiltZ);
    Debug.Plot("error",PID.Out);
    Debug.Send();
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
    delay(10);
    
}

