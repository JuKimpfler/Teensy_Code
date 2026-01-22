#include  "Robot.h"
RobotC Robot;

elapsedMicros Drive_Smoothed_Timer;

void RobotC::Turn(float Angle){
    PID.setAngle(Angle);
    Motor.On(PID.Out,VR_Motor);
    Motor.On(PID.Out,VL_Motor);
    Motor.On(PID.Out,HR_Motor);
    Motor.On(PID.Out,HL_Motor);
}

void RobotC::Drive_Smoothed_set(float Dir , float Angle , int sollSpeed , int istSpeed){
    Drive_Smoothed_Timer = 0;
    Drive_Smoothed_optimal = sollSpeed;
    Drive_Smoothed_start = istSpeed;
    Drive_Smoothed_aktive = true;
}

void RobotC::Drive_Smoothed_Update(){
    if(Drive_Smoothed_aktive==true){
        Drive_Smoothed_diff = Drive_Smoothed_optimal-Drive_Smoothed_start;
        newSpeed = Drive_Smoothed_start + ((1/Drive_Smoothed_Faktor)*Drive_Smoothed_Timer/1000000) * Drive_Smoothed_diff;
        Drive_Smoothed_diffnow = abs(Drive_Smoothed_optimal - newSpeed);
        if(Drive_Smoothed_diffnow > 2){
            Drive_Smoothed_aktive = false;
        }
        Drive(Drive_Smoothed_Dir,Drive_Smoothed_Angle,newSpeed);
        Drive_Smoothed_last = newSpeed;
    }
}

void RobotC::Drive_Smoothed_Kill(){
    Drive_Smoothed_aktive=false;
}

void RobotC::Drive(float Dir , float Angle = 200000 ,int Speed1 = HighSpeed){
    // Links -> Minus (Counter-Clock)
    // Rechts -> Plus (Clock)

    /*
    if (Angle == 200000){
        Angle = Goal_Calc.Angle;
    }
    */
    PID.setAngle(Angle);
    Dir_A = Dir - Angle;

    Vel[VR_Motor] = sinf((Dir_A+45)*DEG_TO_RAD)*15 + (-PID.Out);
    Vel[VL_Motor] = sinf((Dir_A+315)*DEG_TO_RAD)*15 + (-PID.Out);
    Vel[HR_Motor] = sinf((Dir_A+135)*DEG_TO_RAD)*15 + (-PID.Out);
    Vel[HL_Motor] = sinf((Dir_A+225)*DEG_TO_RAD)*15 + (-PID.Out);

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

    if (Dir == 1000){
        Stop();
    }
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
    digitalWrite(18,LOW);
}

void RobotC::KickerC::On(int Cycletime){
    
}

void RobotC::KickerC::Once(){
    digitalWrite(18,HIGH);
    delay(15);
    digitalWrite(18,LOW);
    delay(1000);
}