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

    #ifdef Robo_s 
        System.Calibrate("s");
        VR_Motor = VR_Motors; 
        VL_Motor = VL_Motors; 
        HR_Motor = HR_Motors; 
        HL_Motor = HL_Motors;
        LDR_Schwelle = LDR_Schwelle_s ;
        Motor.Enable = true;
    #endif
    #ifdef Robo_w
        System.Calibrate("w");
        Motor.Enable = true;
    #endif

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


    // Interface

    // Switches
    if (System.Switches[0]){MainSpeed = HighSpeed;}
    else {MainSpeed = LowSpeed;}

    if (System.Switches[1]){}
    else{}

    // Buttons
    if(System.Button[0]){BNO055.Calibrate();} // BNO055 set to 0

    if (System.Button[1]){Robot.Kicker.On();} // Kicker test
    else{Robot.Kicker.Off();}

    if (System.Button[2]){} 
    else{}

    if (System.Switches[2]){if (System.Button[3]){Line.Calibrate(2);Line.Calibrate(3);}} // Wenn SW3 == true: Line_Calibration mit Linie
    else{Line.Calibrate(1);Line.Calibrate(3);} // Wenn SW3 == false: Line_Calibration ohne Linie
    

    // Timer / Cycler
    Cycle_P2++;
    Cycle_P3++;
    Cycletime = Cycle_Timer;
}

