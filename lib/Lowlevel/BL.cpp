#include "BL.h"
#include "RGB.h"
#include "IR.h"

BLC BL;

elapsedMillis BL_Timer;

void BLC::doRolle(){
    int avail = UART_Pixy.available();
    if(avail>0){
        message = UART_Pixy.readStringUntil('$');
        Serial.println("read : "+message);
        if(message.length()==4){
            Serial.println("code : "+decode(message));
        }
        //Dist_Player2 = decode(message).toDouble();
        BL_Timer = 0;
    }
    //Serial.println(String(BL_Timer)+" <- Timer / available -> "+String(avail));

    if(BL_Timer>3000){SinglePlayer = true;}
    else{SinglePlayer = false;}

    if(SinglePlayer = false){
        if(Dist_Player2<Ball.Distance){Rolle="D";}
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

String BLC::decode(String message1){
    //Serial.println("Message:"+String(message1));
    return message1+"decode";
}

