#include "BL.h"
#include "RGB.h"
#include "IR.h"

BLC BL;

elapsedMillis BL_Timer;

void BLC::doRolle(){
    if(UART_Pixy.available()>0){
        message = UART_Pixy.readStringUntil('$');
        Dist_Player2 = decode(message).toFloat();
        BL_Timer = 0;
    }

    if(BL_Timer>3000){SinglePlayer = true;}
    else{SinglePlayer = false;}

    if(SinglePlayer = false){
        if(Dist_Player2<IR.Distance){Rolle="D";}
        else {Rolle="A";}
    }
    else{Rolle="N";}

    showRolle();
}

void BLC::showRolle(){
    if (Rolle=="A"){RGB.write(1,"R");}// LED AN
    else if (Rolle=="B"){RGB.write(1,"G");}// LED AN
    else if (Rolle=="N"){RGB.write(1,"B");}// LED AN
}

String BLC::decode(String message){
    Serial.println("Message:"+String(message));
    return message;
}

