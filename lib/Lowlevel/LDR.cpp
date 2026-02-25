#include "LDR.h"

LDRC LDR;

void LDRC::init(){
    pinMode(LDR_Port,INPUT);
}

bool LDRC::Aktiv(bool check = false){
    if (analogRead(LDR_Port) > LDR_Schwelle){
        return true;
    }
    else{
        return false;
    }
}