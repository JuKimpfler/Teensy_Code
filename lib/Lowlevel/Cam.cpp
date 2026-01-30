#include "Cam.h"
CamC Cam;

void CamC::init(){
    UART_1.begin(115200);
}

void CamC::read(){
    if (UART_1.available()>1){
        message = UART_1.readStringUntil('$');
    
        if (message.length() == 20){
            UART_Debug.println(message);
            Decode(message);
        }
        else{
            UART_Debug.println("E : "+String(message)); // %tt 1000 1000 1000 1000
        }
    }
    Decode(message);
}

void CamC::Decode(String message){

    if (!(message.substring(0,2) == "tt" || message.substring(0,2) == "ff"))
    {
        if (message.substring(0,2) == "tf"){
            goal = true; // gelb
        }
        else{
            goal = false; // blau
        }
        x = ((message.substring(2,6)).toInt());
        y = ((message.substring(6,10)).toInt());
        area = ((message.substring(10,14)).toInt());
    }
}