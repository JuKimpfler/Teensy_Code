#include "BL.h"
#include "RGB.h"
#include "IR.h"
#include "Robot.h"
#include "Mouse.h"

BLC BL;

elapsedMillis BL_Timer_RX;
elapsedMillis BL_Timer_TX;

/**
 * @brief Reads data from UART_Pixy, decodes the message, updates the robots state and sends data back to the Pixy.
 * 
 * @details Reads data from UART_Pixy, decodes the message and updates the robots state. If the robot is in start mode and the mouse is above the surface, it sends the ball angle and distance to the Pixy. Otherwise it sends 9999 for both values. Additionally, it sends a boolean value for info1 and info2, which are currently just example values, and an integer value for extra, which is also currently just an example value.
 * 
 * @note The function is split into three parts: Reading data from UART_Pixy, sending data to the Pixy and updating the robots state.
 */
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
        int angle; //= (Robot.Start && Mouse.lift) ? (int)round(Ball.Angle) : 9999;
        int dist; //= (Robot.Start && Mouse.lift) ? (int)round(Ball.Distance) : 9999;
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
    if((Ball.Angle_P2 == 9999 && Ball.Distance_P2 == 9999)||(BL_Timer_RX>3000)){SinglePlayer = true;}
    else{SinglePlayer = false;}

    if(!SinglePlayer){
        if(Ball.Distance_P2 < Ball.Distance){Rolle="D";}
        else {Rolle="A";}
    }
    else{Rolle="N";}

    showRolle();
}

/**
 * @brief Schreibt die aktuelle Rolle an
 * 
 * @details Schreibt die aktuelle Rolle an, indem die entsprechende Farbe auf dem LED schreibt.
 * "A" = Rot, "D" = Grün, "N" = Blau
 */
void BLC::showRolle(){
    if (Rolle=="A"){RGB.write(1,"R");}
    else if (Rolle=="D"){RGB.write(1,"G");}
    else if (Rolle=="N"){RGB.write(1,"B");}
}


/**
 * @brief Decodes a message received from the Pixy
 * 
 * @details Decodes a message received from the Pixy into its individual values. The message format is 0AAA0DDD0I10I20EEE$, where AAA is the angle, DDD is the distance, I is info1, 10 is info2, and EEE is extra.
 * @param message1 The message to be decoded
 */
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

