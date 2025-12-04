#include "Cam.h"
CamC Cam;

void CamC::init(){
    Serial6.begin(115200);
}

void CamC::read(){
    if (UART_1.available()>1){
        message = UART_1.readStringUntil('$');
    
    if (message.length() == 20){
        UART_Debug.println(message);
        Decode(message);
    }
    else{
        UART_Debug.println("E : "+String(message));
    }
    }
}

String CamC::Decode(String message){
    String message_new = message;
    return message_new;
}