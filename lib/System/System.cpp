#include "System.h"
#include "Debug.h"
#include "Game_Thread.h"
#include "BL.h"

SystemC System;

elapsedMillis Interface_Timer;
elapsedMillis Schuss_Timer;
elapsedMicros Cycle_Timer;
elapsedMicros Mess_Timer;


void SystemC::Start_Update(){
    System.Start = digitalRead(Start_Port);
}

void SystemC::Button_Update(){
    Expander.I2C.read(I2C_ITF_Main);

    System.Button[0] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT0);
    System.Button[1] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT1);
    System.Button[2] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT2);
    System.Button[3] = !Expander.I2C.give(I2C_ITF_Main,ITF_Main_BT3);

    System.Switches[0] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW0);
    System.Switches[1] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW1);
    System.Switches[2] = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW2);
    
}

void SystemC::init(){
    
    //Wire1.setClock(100000);

    pinMode(Start_Port,INPUT);
    pinMode(Kicker_Port, OUTPUT);
    LDR.init(); 

    Line.init();
    Mouse.init();

    Motor.init();
    RGB.init();

    Wire1.begin();
    //Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);
     
    BNO055.init(); 
    delay(10);
    BNO055.Calibrate();

    //US.init();
}

void SystemC::UpdateC::Interface(){
    if (System.Start){
        //System.Button_Update(); // 588
    }
}

void SystemC::UpdateC::Sensors(){
    System.Start_Update(); // 1 micro
    Line.read_Fast(); // 60 micro
    //US.read(); // 1 micro
    Mouse.read(); // 177 micros
    Cam.read(); // 1 micro

    Robot.Kicker.Update(); // 1 micro

    LineCalc.Calc(); // 1 micro
    BallCalc.CalcDist(); // 1 micro
    BallCalc.getAngle(); // 1 micro
    PID.Calculate(); // 1 micro

    if ((Cycle_P2 == 0)){
        BNO055.read(); // 500-1000 micro
    }
    else if ((Cycle_P2 == 1)){
        //IR.read(); // 1300 micro
        Cycle_P2=0;
    }

    if ((Cycle_P3 > 10) && (true)){
        RGB.Apply(); // 300

        Debug.Start();
        Debug.Plot("Ball_Angle",BNO055.TiltZ);
        //Debug.Plot("Ball_raw",Ball.Distance_raw);
        //Debug.Plot("Ball_dist",Ball.Distance);
        Debug.Plot("Drive",BallCalc.DriveAngle);
        //Debug.Plot("fa",IR.DistFaktor);
        Debug.Send();
        Cycle_P3 = 0;
    }
}