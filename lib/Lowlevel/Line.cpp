#include "Line.h"

LineC Line;

void LineC::read(){
    Summe = 0;
    for(int y = 0 ; y<4 ; y++){
        for(int i = 0 ; i<8 ; i++){
            Raw[(y*8)+i] = Expander.ADC.give_digital(list[y],i);
            Summe = Summe + Raw[(y*8)+i];
        }
    }
}

void LineC::read_VW(){
    Summe_VW = 0;
    for(int i = 0 ; i<8 ; i++){
        Raw_VW[i] = Expander.ADC.give_digital(CS_LineVW,i);
        Summe_VW = Summe_VW + Raw_VW[i];
    }
}

