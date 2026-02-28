#include "Motor.h"

MotorC Motor;

void MotorC::init(){
    pinMode(VR_Speed_Port,OUTPUT);
    pinMode(VL_Speed_Port,OUTPUT);
    pinMode(HR_Speed_Port,OUTPUT);
    pinMode(HL_Speed_Port,OUTPUT);

    pinMode(VL_ExpanderA,OUTPUT);
    pinMode(HR_ExpanderA,OUTPUT);
    pinMode(HL_ExpanderA,OUTPUT);
    pinMode(VR_ExpanderA,OUTPUT);

    pinMode(VL_ExpanderB,OUTPUT);
    pinMode(HR_ExpanderB,OUTPUT);
    pinMode(HL_ExpanderB,OUTPUT);
    pinMode(VR_ExpanderB,OUTPUT);

    pinMode(Kicker_Port, OUTPUT);

    analogWriteFrequency(VR_Speed_Port,Motor_Frequency);
    analogWriteFrequency(VL_Speed_Port,Motor_Frequency);
    analogWriteFrequency(HR_Speed_Port,Motor_Frequency);
    analogWriteFrequency(HL_Speed_Port,Motor_Frequency);
}

void MotorC::On(int Speed , int Motor){
    if (Enable){ 

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
            digitalWriteFast(VR_ExpanderA,0);
            digitalWriteFast(VR_ExpanderB,1);
        }
        else if (Motor == VL_Motor){
            analogWrite(VL_Speed_Port,Speed);
            digitalWriteFast(VL_ExpanderA,0);
            digitalWriteFast(VL_ExpanderB,1);
        }
        else if (Motor == HR_Motor){
            analogWrite(HR_Speed_Port,Speed);
            digitalWriteFast(HR_ExpanderA,0);
            digitalWriteFast(HR_ExpanderB,1);
        }
        else if (Motor == HL_Motor){
            analogWrite(HL_Speed_Port,Speed);
            digitalWriteFast(HL_ExpanderA,0);
            digitalWriteFast(HL_ExpanderB,1); 
        }
    }
    else{
        if (Motor == VR_Motor){
            analogWrite(VR_Speed_Port,Speed);
            digitalWriteFast(VR_ExpanderA,1);
            digitalWriteFast(VR_ExpanderB,0);
        }
        else if (Motor == VL_Motor){
            analogWrite(VL_Speed_Port,Speed);
            digitalWriteFast(VL_ExpanderA,1);
            digitalWriteFast(VL_ExpanderB,0);
        }
        else if (Motor == HR_Motor){
            analogWrite(HR_Speed_Port,Speed);
            digitalWriteFast(HR_ExpanderA,1);
            digitalWriteFast(HR_ExpanderB,0);
        }
        else if (Motor == HL_Motor){
            analogWrite(HL_Speed_Port,Speed);
            digitalWriteFast(HL_ExpanderA,1);
            digitalWriteFast(HL_ExpanderB,0); 
        }
    }

    }
}

void MotorC::Off(int Motor){
    if (Motor == VR_Motor){
        analogWrite(VR_Speed_Port,0);
        digitalWriteFast(VR_ExpanderA,0);
        digitalWriteFast(VR_ExpanderB,0);
    }
    else if (Motor == VL_Motor){
        analogWrite(VL_Speed_Port,0);
        digitalWriteFast(VL_ExpanderA,0);
        digitalWriteFast(VL_ExpanderB,0);
    }
    else if (Motor == HR_Motor){
        analogWrite(HR_Speed_Port,0);
        digitalWriteFast(HR_ExpanderA,0);
        digitalWriteFast(HR_ExpanderB,0);
    }
    else if (Motor == HL_Motor){
        analogWrite(HL_Speed_Port,0);
        digitalWriteFast(HL_ExpanderA,0);
        digitalWriteFast(HL_ExpanderB,0); 
    }
}



void MotorC::Break(int Motor){
    if (Motor == VR_Motor){
        analogWrite(VR_Speed_Port,255);
        digitalWriteFast(VR_ExpanderA,0);
        digitalWriteFast(VR_ExpanderB,0);
    }
    else if (Motor == VL_Motor){
        analogWrite(VL_Speed_Port,255);
        digitalWriteFast(VL_ExpanderA,0);
        digitalWriteFast(VL_ExpanderB,0);
    }
    else if (Motor == HR_Motor){
        analogWrite(HR_Speed_Port,255);
        digitalWriteFast(HR_ExpanderA,0);
        digitalWriteFast(HR_ExpanderB,0);
    }
    else if (Motor == HL_Motor){
        analogWrite(HL_Speed_Port,255);
        digitalWriteFast(HL_ExpanderA,0);
        digitalWriteFast(HL_ExpanderB,0); 
    }
}

