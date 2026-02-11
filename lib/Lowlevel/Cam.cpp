#include "Cam.h"
#include "RGB.h"
#include "BNO055.h"
CamC Cam;
GoalC Goal;

void CamC::read(){
    if (UART_2.available()>0){
        message = UART_2.readStringUntil('$');
    
        if (message.length() == 19){
            Decode(message);
        }
    }
}

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
        if(!Goal.Angle<0){Goal.lastdir = false;}
        Goal.Angle = BNO055.give_TiltZ()+((x-140)/3);
    }
    else{
        Goal.Angle = 0;
    }

}