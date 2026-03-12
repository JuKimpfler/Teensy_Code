#include "System.h"
#include "Debug.h"
#include "Game_Thread.h"
#include "BL.h"

SystemC System;

elapsedMillis Reg_Timer;
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

void SystemC::Calibrate(String NR){
    if(NR == "w"){
        LDR_Schwelle = LDR_Schwelle_w;
        IR.Dist_Offset = IR_Dist_Offset_w;
        IR.Angle_Offset = 0;
    }
    if(NR == "s"){
        LDR_Schwelle = LDR_Schwelle_s;
        IR.Dist_Offset = IR_Dist_Offset_s;
        IR.Angle_Offset = 0;
    }
}

void SystemC::init(){
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);

    pinMode(Start_Port,INPUT);
    pinMode(Kicker_Port, OUTPUT);
    LDR.init(); 

    Line.init();

    Motor.init();
    RGB.init();
    BNO055.init(); 
    
    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);

    IR.init();
}

void SystemC::UpdateC::Interface(){
    if (!System.Start){
        System.Button_Update(); // 588
    }
}

void SystemC::UpdateC::printPID() {
    Serial.print("Kp: "); Serial.print(Kp);
    Serial.print(" | Ki: "); Serial.print(Ki);
    Serial.print(" | Kd: "); Serial.print(Kd);
    Serial.print(" | PID_Mult: "); Serial.println(PID_Mult);
}


void SystemC::UpdateC::Sensors(){
    System.Start_Update(); // 1 micro
    Line.read_Fast(); // 60 micro
    US.read(); // 1 micro
    Cam.read(); // 1 micro
    LineCalc.Calc(); // 15 micro
    BallCalc.CalcDist(); // 1 micro
    BallCalc.CalcAngle();
    BallCalc.getAngle(); // 1 micro
    PID.Calculate(); // 1 micro
    BNO055.read(); // 100-500 micro
    IR.read(); // 300 micro
    BL.doRolle();

    if ((Cycle_P3 > 50)){
        #ifdef Debug_EN
        #ifdef Ir_Calib // IR_Calibration 
        Debug.Start();
        Debug.Plot_List("IR",IR.IR_Values_raw,16);
        Debug.Send();
        #endif
        #ifdef Calib // Normal_Calibration
        Debug.Start();
        Debug.Plot("IR_Offset",Ball.Distance_raw2);
        Debug.Plot("Dist",Ball.Distance);
        Debug.Plot("Zone",Zone);
        Debug.Plot("LDR_Analog",analogRead(LDR_Port));
        Debug.Plot("Line_G",Line_Grass);
        Debug.Plot("Line_G_VW",Line_Grass_VW);
        Debug.Plot("Line_N",Line_Norm);
        Debug.Plot("Line_N_VW",Line_Norm_VW);
        Debug.Plot("Line_S",Line_Schwelle);
        Debug.Plot("Line_S_VW",Line_Schwelle_VW);
        Debug.Send();
        #endif
        #ifdef Line_Calib
        Debug.Start();
        Debug.Plot_List("Line", Line.Values_raw, 8);
        Debug.Plot_List("LV", Line.Values_raw_VW, 8);
        //Debug.Plot_List("L", Line.line, 32);
        //Debug.Plot("angle",LineCalc.DriveAngle);
        //Debug.Plot("LVW",Line_Schwelle_VW);
        //Debug.Plot("L",Line_Schwelle);
        Debug.Send();
        #endif
        #ifdef PID_Calib
        if (Serial.available()) {
            String input = Serial.readStringUntil('\n');
            input.trim();
            if (input.startsWith("kp ")) {Kp = input.substring(3).toFloat();Serial.print("Neues Kp: "); Serial.println(Kp);} 
            else if (input.startsWith("ki ")) {Ki = input.substring(3).toFloat();Serial.print("Neuer Ki: "); Serial.println(Ki);} 
            else if (input.startsWith("kd ")) {Kd = input.substring(3).toFloat();Serial.print("Neuer Kd: "); Serial.println(Kd);} 
            else if (input.startsWith("an ")) {inputAngle = input.substring(3).toFloat();Serial.print("Neuer Angle: "); Serial.println(inputAngle);} 
            else if (input.startsWith("mult ")) {PID_Mult = input.substring(5).toFloat();Serial.print("Neuer PID_Mult: "); Serial.println(PID_Mult);} else if (input == "show") {printPID();} 
            else {Serial.println("Unbekannter Befehl. Nutze: kp <wert>, ki <wert>, kd <wert>, mult <wert>, show");}
        }
        #endif
        #ifndef Ir_Calib // Game_Debug
        #ifndef Calib 
        #ifndef Line_Calib
        #ifndef PID_Calib
        Debug.Start();
        Debug.Plot("USL", US.Distance_raw[0]);
        Debug.Plot("USH", US.Distance_raw[1]);
        Debug.Plot("USR", US.Distance_raw[2]);
        Debug.Plot("Ball_angle",Ball.Angle);
        Debug.Plot("Ball_Dist",Ball.Distance);
        Debug.Plot("sight",Ball.inSight);
        Debug.Plot("goal_angle",Goal.Angle);
        Debug.Plot("overload",abs(Goal.Angle-BNO055.TiltZ));
        Debug.Plot("regeln",LineCalc.DriveAngle);
        Debug.Plot("ball_dist2",Ball.Distance_raw);
        Debug.Plot("Rolle",MainSpeed);
        Debug.Send();
        //BL.sendDebug("BNO: " + String(BNO055.TiltZ));
        #endif
        #endif
        #endif
        #endif 
        #endif
        Cycle_P3 = 0;
    }

    #ifdef PID_Calib
    Robot.Turn(999);
    #endif
}