#include "BL.h"
#include "RGB.h"
#include "IR.h"
#include "Robot.h"
#include "Mouse.h"

BLC BL;

elapsedMillis BL_Timer_RX;
elapsedMillis BL_Timer_TX;

void BLC::doRolle(){
    int avail = UART_Pixy.available();
    if(avail>0){
        message = UART_Pixy.readStringUntil('$');
        Serial.println("read : "+message);
        if(message.length()==14){
            decode(message);
        }
        BL_Timer_RX = 0;
    }

    // Senden alle 10ms
    if(BL_Timer_TX > 10){
        int angle = (Robot.Start && Mouse.lift) ? (int)round(Ball.Angle) : 9999;
        int dist = (Robot.Start && Mouse.lift) ? (int)round(Ball.Distance) : 9999;
        bool info1 = true; // Beispielwert, anpassen!
        bool info2 = false; // Beispielwert, anpassen!
        int extra = 123; // Beispielwert, anpassen!
        // Format: 0AAA0DDD0I10I20EEE$
        char sendMsg[16];
        snprintf(sendMsg, sizeof(sendMsg), "0%04d0%04d0%d0%d0%03d$", angle, dist, info1, info2, extra);
        UART_Pixy.print(sendMsg);
        BL_Timer_TX = 0;
    }

    // SinglePlayer, wenn P2-Daten 9999
    if(Ball.Angle_P2 == 9999 && Ball.Distance_P2 == 9999){SinglePlayer = true;}
    else{SinglePlayer = false;}

    if(!SinglePlayer){
        if(Ball.Distance_P2 < Ball.Distance){Rolle="D";}
        else {Rolle="A";}
    }
    else{Rolle="N";}

    showRolle();
}

void BLC::showRolle(){
    if (Rolle=="A"){RGB.write(1,"R");}
    else if (Rolle=="D"){RGB.write(1,"G");}
    else if (Rolle=="N"){RGB.write(1,"B");}
}

void BLC::decode(String message1){
    // 0AAA0DDD0I10I20EEE$
    if(message1.length() < 14) return;
    int angle = message1.substring(1,5).toInt();
    int dist = message1.substring(6,10).toInt();
    bool info1 = message1.substring(11,12).toInt();
    bool info2 = message1.substring(13,14).toInt();
    int extra = message1.substring(15,18).toInt();
    Ball.Angle_P2 = angle;
    Ball.Distance_P2 = dist;
    // info1, info2, extra können hier weiterverarbeitet werden
}

