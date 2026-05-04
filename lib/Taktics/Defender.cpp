#include "Defender.h"

DefenderC Defender;

void DefenderC::Bump_Ball(){
    if(Ball.Stilltime > 1500){
        Robot.Drive(Ball.Angle,0,40);
    }
}

float DefenderC::Follow_Line(){
    float mdiff = 200;
    float diff = 0;
    float Position = 0;
    float mPosition = 0;
    int written = false;

    Serial.print("start:  ");

    for (int i = 0; i<32 ; i++){
        if(Line.line[i]){
            Position = U.Circel(((i*11.25)-90)*1);
            diff = abs(U.Circel(Position-Ball.Angle));
    
            Serial.print(Position);
            Serial.print(" ~ ");
            Serial.print(diff);
            Serial.print(" , ");

            if(diff<mdiff){
                mdiff = diff;
                mPosition = Position;
                written = 1;
            }
        }
    }

    Serial.println(" ");

    if(written == 0){
        return 0;
    }
    else if(written == 1){
        Serial.println(mPosition);
        return (mPosition*-1);
    }

    if(Line.Summe == 0){
        return 180;
    }

}

void DefenderC::set_State(bool State){
    State_Def = State;
}

void DefenderC::init(){
    
}

void DefenderC::Update(){
    if(State_Def == true){
        Robot.Drive(Follow_Line(),0,15);
        Bump_Ball();
    }
}