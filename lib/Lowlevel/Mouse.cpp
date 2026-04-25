#include "Mouse.h"
#include "BNO055.h"

MouseC Mouse;
PMW3389 pmw = PMW3389(&SPI, Mouse_Port); // SPI CS vom Moussensor kommt auf PixyTx

void MouseC::init(){
    delay(1000);
    pmw.begin();
    delay(50);
}

void MouseC::read(){      
    PMW3389_DATA mouseData = pmw.readBurst();

    deltaX = mouseData.dx * 1.25 / 100;
    deltaY = mouseData.dy * 1.25 / 100;

    //delta_dist = sqrtf(pow(deltaY,2)+pow(deltaX,2));
    //delta_angle = atan2f(deltaY,deltaX);

    xPos = xPos - (deltaX * cos(BNO055.giveRad())) + (deltaY * sin(BNO055.giveRad()));
    yPos = yPos - (deltaX * sin(BNO055.giveRad())) + (deltaY * cos(BNO055.giveRad()));

    lift = mouseData.isOnSurface;
    movement = mouseData.isMotion;
}


float MouseC::giveVx(){
    return yPos;
}


float MouseC::giveVy(){
    return xPos;
}

