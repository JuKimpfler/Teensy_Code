#include "Elementar.h"
#include "System.h"
#include "Cam.h"
#include "RGB.h"
#include "Mouse.h"
#include "US.h"
#include "Debug.h"
#include "BL.h"
#include "Game_Thread.h"
#include "BotConnect.h"

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
        Motor.Enable = true;
    #endif
    #ifdef Robo_w
        System.Calibrate("w");
        Motor.Enable = true;
    #endif

    System.init();

    delay(1000);
    
    #ifdef Com_Debug
        BL.setDebugEnabled(true);
    #endif
}

void loop() { 
    Robot.Kicker.Off();
    Cycle_Timer = 0 ;

    System.Update.Sensors();
    System.Update.Interface();
    
    #ifndef PID_Calib
    if(System.Start){ 
        Game.Run();
        //DefenderTactics.Homing();
        //Robot.Turn(999, MainSpeed); 
        
        //Taktics.BallSearch();
        //Robot.Drive(0,90,20);
        //Motor.On(100,VR_Motor);
        //Motor.On(100,VR_Motor);
        //Motor.On(100,HL_Motor);
        //Motor.On(100,HR_Motor);
    }
    else{
        Robot.Stop();
    }
    #endif

    // Interface

    // Switches
    if (System.Switches[0]){MainSpeed = HighSpeed;}
    else {MainSpeed = LowSpeed;}

    Goal_Turn = System.Switches[1];

    // Buttons
    #ifndef Ir_Calib
    if(System.Button[0]){BNO055.Calibrate();Line.Calibrate(1);} // BNO055 set to 0

    if (System.Button[1]){Robot.Kicker.On();} // Kicker test
 
    if (System.Button[2]){IR.Calib_Dist();} 
    else{}
    #endif
    #ifdef Ir_Calib
    if(System.Button[0]){IR.Calib_Dist();} // BNO055 set to 0

    if (System.Button[1]){IR.Calib_Offset();} // Kicker test

    if (System.Button[2]){} 

    if(System.Button[3]){IR.Save();}
    #endif

    //if (System.Switches[2]){if (System.Button[3]){Line.Calibrate(2);}} // Wenn SW3 == true: Line_Calibration mit Linie
    //else{if (System.Button[3]){Line.Calibrate(1);}} // Wenn SW3 == false: Line_Calibration ohne Linie

    // Timer / Cycler
    Cycle_P2++;
    Cycle_P3++;
    Cycletime = Cycle_Timer;
    Robot.Kicker.Update(); // 1 micro
}

