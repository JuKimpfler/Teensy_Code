#include "IR.h"

IRC IR;

void IRC::read(){
    IR_Values[0] = Expander.ADC.give(CS_IR,0);
    IR_Values[1] = Expander.ADC.give(CS_IR,1);
    IR_Values[2] = Expander.ADC.give(CS_IR,2);
    IR_Values[3] = Expander.ADC.give(CS_IR,3);
    IR_Values[4] = Expander.ADC.give(CS_IR,4);
    IR_Values[5] = Expander.ADC.give(CS_IR,5);
    IR_Values[6] = Expander.ADC.give(CS_IR,6);
    IR_Values[7] = Expander.ADC.give(CS_IR,7);
}

int IRC::GetData(int Port){
    return IR_Values[Port];
}