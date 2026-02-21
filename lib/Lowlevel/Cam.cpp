#include "Cam.h"
#include "RGB.h"
#include "BNO055.h"
CamC Cam;
GoalC Goal;

/**
 * @brief Reads a message from the serial port and decodes it if it is a valid message.
 * 
 * @details Checks if there is a message available in the serial port. If there is, it reads the message and checks if it is a valid message (19 characters long). If it is, it decodes the message and updates the goal and ball positions.
 */

void CamC::read(){
    if (UART_2.available()>0){
        message = UART_2.readStringUntil('$');
    
        if (message.length() == 19){
            Decode(message);
        }
    }
}

/**
 * @brief Decodes a message from the serial port and updates the goal and ball positions.
 * 
 * @details Checks if the message is valid (19 characters long and contains "tt" or "ff" as the first three characters). If it is, it decodes the message and updates the goal and ball positions. If not, it sets the goal and ball positions to default values.
 * 
 * @param message The message to decode.
 */

void CamC::Decode(String message){

    if (!(message.substring(1,3) == "tt" || message.substring(1,3) == "ff"))
    {
        if (message.substring(1,3) == "tf"){
            goal = true;
            RGB.write(2,"Y");
            Goal.inSight = false;
        }
        else if (message.substring(1,3) == "ft"){
            goal = false;
            RGB.write(2,"B");
            Goal.inSight = true;
        }
        x = ((message.substring(3,7)).toInt());
        y = ((message.substring(7,11)).toInt());
        area = ((message.substring(11,15)).toInt())*25; // daten übertragung mit geminderter auflösung um große zahlen darzustellen
        rest = ((message.substring(15,19)).toInt());
    }
    else { 
        RGB.write(2,"R");
        x = 0;
        y = 0;
        area = 0;
        rest = 0;
        Goal.inSight = false;
    }
    

    Goal.Y = y;
    Goal.X = x;
    Goal.Area = area;
    if(Goal.inSight==true){
        if(Goal.Angle<0){Goal.lastdir = true;}
        if(Goal.Angle >= 0){Goal.lastdir = false;}
        //Goal.Angle = BNO055.TiltZ+((x-140)/3);
    }
    else{
        Goal.Angle = 0;
    }

}