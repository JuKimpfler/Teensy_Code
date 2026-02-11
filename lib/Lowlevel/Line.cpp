#include "Line.h"

LineC Line;

void LineC::read(){
    Summe = 0;
    for(int y = 0 ; y<4 ; y++){
        for(int i = 0 ; i<8 ; i++){
            if(((y*8)+i) == 13){ // pt 13 kaputt
                Raw[13] = Expander.ADC.give(list[1],4);
                Summe = Summe + Raw[13];
            }
            else{
                Raw[(y*8)+i] = Expander.ADC.give(list[y],i);
                Summe = Summe + Raw[(y*8)+i];
            }
            //Serial.print(" ,"+String((y*8)+i)+": ");
            //Serial.print(Raw[(y*8)+i]);
        }
    }
    //Serial.println("");
}

void LineC::read_VW(){
    Summe_VW = 0;
    for(int i = 0 ; i<8 ; i++){
        Raw_VW[i] = Expander.ADC.give_digital(CS_LineVW,i);
        Summe_VW = Summe_VW + Raw_VW[i];
    }
}

