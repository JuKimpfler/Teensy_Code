#include "Robot.h"
#include "RGB.h"
#include "ESC.h"
#include "PU.h"
#include "Line.h"
#include "Cam.h"
RobotC Robot;

elapsedMillis Kicker_Timer;

void RobotC::Turn(float Angle ,int Speed1){
    PID.setAngle(Angle);
    Motor.On(PID.Out,VR_Motor);
    Motor.On(PID.Out,VL_Motor);
    Motor.On(PID.Out,HR_Motor);
    Motor.On(PID.Out,HL_Motor);
}

void RobotC::Drive(float Dir , float Angle,int Speed1){
    Dir = Dir-BNO055.TiltZ;

    if(Color_ID==true){
        Speed1 = Speed_w_faktor*Speed1;
    }

    if(Speed1 > 100){
        Speed1 = 100;
    }

    Dir= U.Circel(Dir)*-1;

    /*if(preLineAvoidance){
        lastDir = Dir;
        Dir = PU.AlongBorder(PU.Positon.x_cm,PU.Positon.y_cm,Dir,5,15);
        lastDircor = Dir;
    }*/
    /*if(preLineDec && Line.Summe != 0){
        if(sqrt(pow(Cam.give_Angle1(),2)+pow(Cam.give_Angle2(),2))>45){
            Speed1 = Speed1*0.2;
        }
        Serial.println("decSpeed");
    }*/

    Angle= U.Circel(Angle);

    PID.setAngle(Angle);

    Dir_A = Dir;

    Vel[VR_Motor] = sinf((Dir_A+55)*DEG_TO_RAD)*15 + (-PID.Out);
    Vel[VL_Motor] = sinf((Dir_A+305)*DEG_TO_RAD)*15 + (-PID.Out);
    Vel[HR_Motor] = sinf((Dir_A+145)*DEG_TO_RAD)*15 + (-PID.Out);
    Vel[HL_Motor] = sinf((Dir_A+215)*DEG_TO_RAD)*15 + (-PID.Out);


    if (Vel[VR_Motor]<0){Vel_D[VR_Motor]=Vel[VR_Motor]*-1;Dir0=0;}
    else{Vel_D[VR_Motor]=Vel[VR_Motor];Dir0=1;}
    if (Vel[VL_Motor]<0){Vel_D[VL_Motor]=Vel[VL_Motor]*-1;Dir1=0;}
    else{Vel_D[VL_Motor]=Vel[VL_Motor];Dir1=1;}
    if (Vel[HR_Motor]<0){Vel_D[HR_Motor]=Vel[HR_Motor]*-1;Dir2=0;}
    else{Vel_D[HR_Motor]=Vel[HR_Motor];Dir2=1;}
    if (Vel[HL_Motor]<0){Vel_D[HL_Motor]=Vel[HL_Motor]*-1;Dir3=0;}
    else{Vel_D[HL_Motor]=Vel[HL_Motor];Dir3=1;}

    // Werte Mappen auf "Speed"
    if ((Vel_D[VR_Motor]>=Vel_D[VL_Motor]) && (Vel_D[VR_Motor]>=Vel_D[HR_Motor]) && (Vel_D[VR_Motor]>=Vel_D[HL_Motor])){
        Error = Speed1/Vel_D[VR_Motor];
        Vel_D[VR_Motor]= Vel_D[VR_Motor]*Error;
        Vel_D[VL_Motor]= Vel_D[VL_Motor]*Error;
        Vel_D[HR_Motor]= Vel_D[HR_Motor]*Error; 
        Vel_D[HL_Motor]= Vel_D[HL_Motor]*Error; 
    }
    if ((Vel_D[VL_Motor]>=Vel_D[VR_Motor]) && (Vel_D[VL_Motor]>=Vel_D[HR_Motor]) && (Vel_D[VL_Motor]>=Vel_D[HL_Motor])){
        Error = Speed1/Vel_D[VL_Motor];
        Vel_D[VR_Motor]= Vel_D[VR_Motor]*Error;
        Vel_D[VL_Motor]= Vel_D[VL_Motor]*Error;
        Vel_D[HR_Motor]= Vel_D[HR_Motor]*Error; 
        Vel_D[HL_Motor]= Vel_D[HL_Motor]*Error; 
    }
    if ((Vel_D[HR_Motor]>=Vel_D[VL_Motor]) && (Vel_D[HR_Motor]>=Vel_D[VR_Motor]) && (Vel_D[HR_Motor]>=Vel_D[HL_Motor])){
        Error = Speed1/Vel_D[HR_Motor];
        Vel_D[VR_Motor]= Vel_D[VR_Motor]*Error;
        Vel_D[VL_Motor]= Vel_D[VL_Motor]*Error;
        Vel_D[HR_Motor]= Vel_D[HR_Motor]*Error; 
        Vel_D[HL_Motor]= Vel_D[HL_Motor]*Error; 
    }
    if ((Vel_D[HL_Motor]>=Vel_D[VL_Motor]) && (Vel_D[HL_Motor]>=Vel_D[HR_Motor]) && (Vel_D[HL_Motor]>=Vel_D[VR_Motor])){
        Error = Speed1/Vel_D[HL_Motor];
        Vel_D[VR_Motor]= Vel_D[VR_Motor]*Error;
        Vel_D[VL_Motor]= Vel_D[VL_Motor]*Error;
        Vel_D[HR_Motor]= Vel_D[HR_Motor]*Error; 
        Vel_D[HL_Motor]= Vel_D[HL_Motor]*Error; 
    }

    if (Dir0==1){Vel_D[VR_Motor] = Vel_D[VR_Motor]*-1;}
    if (Dir1==1){Vel_D[VL_Motor] = Vel_D[VL_Motor]*-1;}
    if (Dir2==1){Vel_D[HR_Motor] = Vel_D[HR_Motor]*-1;}
    if (Dir3==1){Vel_D[HL_Motor] = Vel_D[HL_Motor]*-1;}

    Motor.On(Vel_D[VR_Motor],VR_Motor);
    Motor.On(Vel_D[VL_Motor],VL_Motor);
    Motor.On(Vel_D[HR_Motor],HR_Motor);
    Motor.On(Vel_D[HL_Motor],HL_Motor);
}

void RobotC::Stop(){
    Motor.Off(VR_Motor);
    Motor.Off(VL_Motor);
    Motor.Off(HR_Motor);
    Motor.Off(HL_Motor);
}

void RobotC::Break(){
    Motor.Break(VR_Motor);
    Motor.Break(VL_Motor);
    Motor.Break(HR_Motor);
    Motor.Break(HL_Motor);
}
void RobotC::KickerC::Off(){
    active = false;
    onceActive = false;
}


void RobotC::KickerC::Update_End(){
    if(set==false){
        active=false;
    }
    set=false;
}

void RobotC::KickerC::Update(){
    // one-shot pulse handling
    if(onceActive){
        if(Kicker_Timer > 150){
            digitalWrite(Kicker_Port, LOW);
            onceActive = false;
        }
        if(Kicker_Timer > 5000){
            // safety: never hold high longer than 1s
            digitalWrite(Kicker_Port, LOW);
            onceActive = false;
        }
    }
    
    // cyclic pulse handling
    if(active){
        if(Kicker_Timer > 150){
            ESC.stop();
            digitalWrite(Kicker_Port, LOW);
        }
        if(Kicker_Timer > cyclet){
            ESC.stop();
            digitalWrite(Kicker_Port, HIGH);
            Kicker_Timer = 0;
        }
    }
}

void RobotC::KickerC::On(int Cycle){
    cyclet = Cycle;
    active = true;
    onceActive = false;
    set = true;
}

void RobotC::KickerC::Once(){
    active = false;
    onceActive = true;
    digitalWrite(Kicker_Port, HIGH);
    Kicker_Timer = 0;
}