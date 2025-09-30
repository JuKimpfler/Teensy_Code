#include "Motor.h"

MotorC Motor;

void MotorC::init(){
    pinMode(VL_Speed_Port,OUTPUT);
    pinMode(HR_Speed_Port,OUTPUT);
    pinMode(HL_Speed_Port,OUTPUT);
    pinMode(DR_Speed_Port,OUTPUT);

    pinMode(Kicker_Port,OUTPUT);

    pinMode(VR_PortA,OUTPUT);
    pinMode(VR_PortB,OUTPUT);
    pinMode(VL_PortA,OUTPUT);
    pinMode(VL_PortB,OUTPUT);
    pinMode(HR_PortA,OUTPUT);
    pinMode(HR_PortB,OUTPUT);
    pinMode(HL_PortA,OUTPUT);
    pinMode(HL_PortB,OUTPUT);
    
    analogWriteFrequency(VR_Speed_Port,Motor_Frequency);
    analogWriteFrequency(VL_Speed_Port,Motor_Frequency);
    analogWriteFrequency(HR_Speed_Port,Motor_Frequency);
    analogWriteFrequency(HL_Speed_Port,Motor_Frequency);

    analogWriteFrequency(DR_Speed_Port,Motor_Frequency);
}

void MotorC::On(int Speed , int Motor){
    Speed = Speed*2.55;
    bool Dir;
    if (Speed < 0){
        Speed = Speed * -1;
        Dir = true;
    }
    else{
        Dir = false;
    }

    if (Dir == true){
        if (Motor == VR_Motor){
            analogWrite(VR_Speed_Port,Speed);
            digitalWrite(VR_PortA,1);
            digitalWrite(VR_PortB,0);
        }
        else if (Motor == VL_Motor){
            analogWrite(VL_Speed_Port,Speed);
            digitalWrite(VL_PortA,1);
            digitalWrite(VL_PortB,0);
        }
        else if (Motor == HR_Motor){
            analogWrite(HR_Speed_Port,Speed);
            digitalWrite(HR_PortA,1);
            digitalWrite(HR_PortB,0);
        }
        else if (Motor == HL_Motor){
            analogWrite(HL_Speed_Port,Speed);
            digitalWrite(HL_PortA,1);
            digitalWrite(HL_PortB,0);
        }

    }
    else{
        if (Motor == VR_Motor){
            analogWrite(VR_Speed_Port,Speed);
            digitalWrite(VR_PortA,0);
            digitalWrite(VR_PortB,1);
        }
        else if (Motor == VL_Motor){
            analogWrite(VL_Speed_Port,Speed);
            digitalWrite(VL_PortA,0);
            digitalWrite(VL_PortB,1);
        }
        else if (Motor == HR_Motor){
            analogWrite(HR_Speed_Port,Speed);
            digitalWrite(HR_PortA,0);
            digitalWrite(HR_PortB,1);
        }
        else if (Motor == HL_Motor){
            analogWrite(HL_Speed_Port,Speed);
            digitalWrite(HL_PortA,0);
            digitalWrite(HL_PortB,1);
        }
    }

    
}

void MotorC::OnFast(int MotorVR , int MotorVL , int MotorHR , int MotorHL){
    MotorVR = MotorVR*-2.55;
    MotorVL = MotorVL*-2.55;
    MotorHR = MotorHR*-2.55;
    MotorHL = MotorHL*-2.55;

    bool DirVR;
    bool DirVL;
    bool DirHR;
    bool DirHL;

    if (MotorVR < 0){MotorVR = MotorVR * -1;DirVR = true;}else{DirVR = false;}
    if (MotorVL < 0){MotorVL = MotorVL * -1;DirVL = true;}else{DirVL = false;}
    if (MotorHR < 0){MotorHR = MotorHR * -1;DirHR = true;}else{DirHR = false;}
    if (MotorHL < 0){MotorHL = MotorHL * -1;DirHL = true;}else{DirHL = false;}

    byte out = 0;
    int Switch[8] = {0,0,0,0,0,0,0,0};

    Switch[VR_PortA] = int(DirVR);
    Switch[VR_PortB] = int(!DirVR);
    Switch[VL_PortA] = int(DirVL);
    Switch[VL_PortB] = int(!DirVL);
    Switch[HR_PortA] = int(DirHR);
    Switch[HR_PortB] = int(!DirHR);
    Switch[HL_PortA] = int(DirHL);
    Switch[HL_PortB] = int(!DirHL);

    analogWrite(VR_Speed_Port,MotorVR);
    analogWrite(VL_Speed_Port,MotorVL);
    analogWrite(HR_Speed_Port,MotorHR);
    analogWrite(HL_Speed_Port,MotorHL);

    for (int i = 0 ; i<8 ; i++){
        digitalWrite(Iteration_list[i],Switch[i]);
    }
}

void MotorC::Off(int Motor){
    if (Motor == VR_Motor){
        analogWrite(VR_Speed_Port,0);
    }
    else if (Motor == VL_Motor){
        analogWrite(VL_Speed_Port,0);
    }
    else if (Motor == HR_Motor){
        analogWrite(HR_Speed_Port,0);
    }
    else if (Motor == HL_Motor){
        analogWrite(HL_Speed_Port,0);
    }
}

void MotorC::OffFast(){
    analogWrite(VR_Speed_Port,0);
    analogWrite(VL_Speed_Port,0);
    analogWrite(HR_Speed_Port,0);
    analogWrite(HL_Speed_Port,0);
}

void MotorC::BreakFast(){
    analogWrite(VR_Speed_Port,255);
    analogWrite(VL_Speed_Port,255);
    analogWrite(HR_Speed_Port,255);
    analogWrite(HL_Speed_Port,255);
    for (int i = 0 ; i<8 ; i++){
        digitalWrite(Iteration_list[i],0);
    }
}

