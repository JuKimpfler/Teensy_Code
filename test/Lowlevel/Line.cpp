#include "Line.h"

LineC Line;

void LineC::read(){
    for(int y = 0 ; y<4 ; y++){
        for(int i = 0 ; i<8 ; i++){
            Raw[(y*8)+i] = Expander.ADC.give_digital(list[y],i);
        }
    }
}

