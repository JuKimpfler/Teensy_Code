#include "Motor.h"

MotorC Motor;

void MotorC::init(){
    pinMode(VR_Speed_Port,OUTPUT);
    pinMode(VL_Speed_Port,OUTPUT);
    pinMode(HR_Speed_Port,OUTPUT);
    pinMode(HL_Speed_Port,OUTPUT);
    pinMode(DR_Speed_Port,OUTPUT);
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
            Expander.I2C.write(I2C_Motor,VR_ExpanderA,0);
            Expander.I2C.write(I2C_Motor,VR_ExpanderB,1);
        }
        else if (Motor == VL_Motor){
            analogWrite(VL_Speed_Port,Speed);
            Expander.I2C.write(I2C_Motor,VL_ExpanderA,1);
            Expander.I2C.write(I2C_Motor,VL_ExpanderB,0);
        }
        else if (Motor == HR_Motor){
            analogWrite(HR_Speed_Port,Speed);
            Expander.I2C.write(I2C_Motor,HR_ExpanderA,1);
            Expander.I2C.write(I2C_Motor,HR_ExpanderB,0);
        }
        else if (Motor == HL_Motor){
            analogWrite(HL_Speed_Port,Speed);
            Expander.I2C.write(I2C_Motor,HL_ExpanderA,0);
            Expander.I2C.write(I2C_Motor,HL_ExpanderB,1); 
        }
        else if (Motor == 4){
            analogWrite(DR_Speed_Port,Speed);
            Expander.I2C.write(I2C_Schuss,DR_ExpanderA,0);
            Expander.I2C.write(I2C_Schuss,DR_ExpanderB,1); 
        }
    }
    else{
        if (Motor == VR_Motor){
            analogWrite(VR_Speed_Port,Speed);
            Expander.I2C.write(I2C_Motor,VR_ExpanderA,1);
            Expander.I2C.write(I2C_Motor,VR_ExpanderB,0);
        }
        else if (Motor == VL_Motor){
            analogWrite(VL_Speed_Port,Speed);
            Expander.I2C.write(I2C_Motor,VL_ExpanderA,0);
            Expander.I2C.write(I2C_Motor,VL_ExpanderB,1);
        }
        else if (Motor == HR_Motor){
            analogWrite(HR_Speed_Port,Speed);
            Expander.I2C.write(I2C_Motor,HR_ExpanderA,0);
            Expander.I2C.write(I2C_Motor,HR_ExpanderB,1);
        }
        else if (Motor == HL_Motor){
            analogWrite(HL_Speed_Port,Speed);
            Expander.I2C.write(I2C_Motor,HL_ExpanderA,1);
            Expander.I2C.write(I2C_Motor,HL_ExpanderB,0); 
        }
        else if (Motor == 4){
            analogWrite(DR_Speed_Port,Speed);
            Expander.I2C.write(I2C_Schuss,DR_ExpanderA,1);
            Expander.I2C.write(I2C_Schuss,DR_ExpanderB,1); 
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

    Switch[VR_ExpanderA] = int(DirVR);
    Switch[VR_ExpanderB] = int(!DirVR);
    Switch[VL_ExpanderB] = int(DirVL);
    Switch[VL_ExpanderA] = int(!DirVL);
    Switch[HR_ExpanderB] = int(DirHR);
    Switch[HR_ExpanderA] = int(!DirHR);
    Switch[HL_ExpanderA] = int(DirHL);
    Switch[HL_ExpanderB] = int(!DirHL);

    analogWrite(VR_Speed_Port,MotorVR);
    analogWrite(VL_Speed_Port,MotorVL);
    analogWrite(HR_Speed_Port,MotorHR);
    analogWrite(HL_Speed_Port,MotorHL);

    for (int i = 0; i < 8; i++) {
        if (Switch[i] == 1) {
            out |= (1 << (7 - i)); // Bit setzen, wenn der Wert 1 ist
        }
    }

    Wire1.beginTransmission(I2C_Motor); //address second port expander
    Wire1.write(Output_Reg);             // select output register
    Wire1.write(out);            // update output ports Port_Seting
    Wire1.endTransmission();
}

void MotorC::Off(int Motor){
    if (Motor == VR_Motor){
        analogWrite(VR_Speed_Port,0);
        Expander.I2C.write(I2C_Motor,VR_ExpanderA,0);
        Expander.I2C.write(I2C_Motor,VR_ExpanderB,0);
    }
    else if (Motor == VL_Motor){
        analogWrite(VL_Speed_Port,0);
        Expander.I2C.write(I2C_Motor,VL_ExpanderA,0);
        Expander.I2C.write(I2C_Motor,VL_ExpanderB,0);
    }
    else if (Motor == HR_Motor){
        analogWrite(HR_Speed_Port,0);
        Expander.I2C.write(I2C_Motor,HR_ExpanderA,0);
        Expander.I2C.write(I2C_Motor,HR_ExpanderB,0);
    }
    else if (Motor == HL_Motor){
        analogWrite(HL_Speed_Port,0);
        Expander.I2C.write(I2C_Motor,HL_ExpanderA,0);
        Expander.I2C.write(I2C_Motor,HL_ExpanderB,0); 
    }
    else if (Motor == 4){
        analogWrite(DR_Speed_Port,0);
        Expander.I2C.write(I2C_Schuss,DR_ExpanderA,0);
        Expander.I2C.write(I2C_Schuss,DR_ExpanderB,0); 
    }
}

void MotorC::OffFast(){
    analogWrite(VR_Speed_Port,255);
    analogWrite(VL_Speed_Port,255);
    analogWrite(HR_Speed_Port,255);
    analogWrite(HL_Speed_Port,255);
    Wire1.beginTransmission(I2C_Motor); //address second port expander
    Wire1.write(Output_Reg);             // select output register
    Wire1.write(All_Off);            // update output ports Port_Seting
    Wire1.endTransmission();
}

void MotorC::BreakFast(){
    analogWrite(VR_Speed_Port,0);
    analogWrite(VL_Speed_Port,0);
    analogWrite(HR_Speed_Port,0);
    analogWrite(HL_Speed_Port,0);
    Wire1.beginTransmission(I2C_Motor); //address second port expander
    Wire1.write(Output_Reg);             // select output register
    Wire1.write(0b01010101);            // update output ports Port_Seting
    Wire1.endTransmission();
}

void MotorC::Break(int Motor){
    if (Motor == VR_Motor){
        analogWrite(VR_Speed_Port,255);
        Expander.I2C.write(I2C_Motor,VR_ExpanderA,0);
        Expander.I2C.write(I2C_Motor,VR_ExpanderB,0);
    }
    else if (Motor == VL_Motor){
        analogWrite(VL_Speed_Port,255);
        Expander.I2C.write(I2C_Motor,VL_ExpanderA,0);
        Expander.I2C.write(I2C_Motor,VL_ExpanderB,0);
    }
    else if (Motor == HR_Motor){
        analogWrite(HR_Speed_Port,255);
        Expander.I2C.write(I2C_Motor,HR_ExpanderA,0);
        Expander.I2C.write(I2C_Motor,HR_ExpanderB,0);
    }
    else if (Motor == HL_Motor){
        analogWrite(HL_Speed_Port,255);
        Expander.I2C.write(I2C_Motor,HL_ExpanderA,0);
        Expander.I2C.write(I2C_Motor,HL_ExpanderB,0); 
    }
    else if (Motor == 4){
        analogWrite(DR_Speed_Port,255);
        Expander.I2C.write(I2C_Schuss,DR_ExpanderA,0);
        Expander.I2C.write(I2C_Schuss,DR_ExpanderB,0); 
    }
}

