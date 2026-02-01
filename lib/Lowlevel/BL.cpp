#include "BL.h"
#include "RGB.h"

BLC BL;

void BLC::init(){
    UART_2.begin(115200);
}

void BLC::doRolle(){
    
}

void BLC::showRolle(){
    if (Rolle==0){RGB.write(1,"R");}// LED AN
    else if (Rolle==1){RGB.write(1,"O");}// LED AN
    else if (Rolle==2){RGB.write(1,"Y");}// LED AN
    else if (Rolle==3){RGB.write(1,"G");}// LED AN
}

String BLC::decode(String message){

}

