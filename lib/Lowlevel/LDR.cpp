#include "LDR.h"

LDRC LDR;

void LDRC::init(){
    pinMode(LDR_Port,INPUT);
}

bool LDRC::Aktiv(bool check ){
    if (analogRead(LDR_Port) > LDR_Schwelle[Color_ID]){
        return true;
    }
    else{
        return false;
    }
}