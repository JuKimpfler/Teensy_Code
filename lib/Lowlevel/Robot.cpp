#include  "Robot.h"
RobotC Robot;

void RobotC::Turn(float Angle ,int Speed1 = HighSpeed){
    PID.setAngle(Angle);
    Motor.On(PID.Out,VR_Motor);
    Motor.On(PID.Out,VL_Motor);
    Motor.On(PID.Out,HR_Motor);
    Motor.On(PID.Out,HL_Motor);
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

    Vel[VR_Motor] = sinf((Dir_A+45)*DEG_TO_RAD) + (PID.Out * PID_Konstante);
    Vel[VL_Motor] = sinf((Dir_A+315)*DEG_TO_RAD) + (PID.Out * PID_Konstante);
    Vel[HR_Motor] = sinf((Dir_A+135)*DEG_TO_RAD) + (PID.Out * PID_Konstante);
    Vel[HL_Motor] = sinf((Dir_A+225)*DEG_TO_RAD) + (PID.Out * PID_Konstante);

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

    if (Dir0==0){Vel_D[VR_Motor] = Vel_D[VR_Motor]*-1;}
    if (Dir1==0){Vel_D[VL_Motor] = Vel_D[VL_Motor]*-1;}
    if (Dir2==0){Vel_D[HR_Motor] = Vel_D[HR_Motor]*-1;}
    if (Dir3==0){Vel_D[HL_Motor] = Vel_D[HL_Motor]*-1;}

    Motor.On(Vel_D[VR_Motor],VR_Motor);
    Motor.On(Vel_D[VL_Motor],VL_Motor);
    Motor.On(Vel_D[HR_Motor],HR_Motor);
    Motor.On(Vel_D[HL_Motor],HL_Motor);
}

void RobotC::Stop(){
    Motor.Break(1);
    Motor.Break(2);
    Motor.Break(3);
    Motor.Break(4);
}

void RobotC::KickerC::Off(){
    
}

void RobotC::KickerC::On(int Cycletime){
    
}

void RobotC::KickerC::Once(){
    
}