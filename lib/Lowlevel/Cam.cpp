#include "Cam.h"
#include "RGB.h"
CamC Cam;

void CamC::read(){
    if (UART_1.available()>0){
        message = UART_1.readStringUntil('$');
    
        if (message.length() == 19){
            Decode(message);
        }
    }
}

void CamC::Decode(String message){

    if (!(message.substring(1,3) == "tt" || message.substring(1,3) == "ff"))
    {
        if (message.substring(1,3) == "tf"){
            goal = true; // gelb
            RGB.write(2,"Y");
        }
        else if (message.substring(1,3) == "ft"){
            goal = false; // blau
            RGB.write(2,"B");
        }
        x = ((message.substring(3,7)).toFloat());
        y = ((message.substring(7,11)).toFloat());
        area = ((message.substring(11,15)).toFloat())*20; // daten übertragung mit geminderter auflösung um große zahlen darzustellen
        rest = ((message.substring(15,19)).toFloat());
    }
    else { 
        RGB.write(2,"R");
        x = 0;
        y = 0;
        area = 0;
        rest = 0;
    }
}