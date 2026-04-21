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

    delta_dist = sqrtf(pow(deltaY,2)+pow(deltaX,2));
    delta_angle = atan2f(deltaY,deltaX);

    if (delta_dist>1){
        delta_dist=1;
    }

    xPos = xPos + deltaX ;//* cos(BNO055.TiltZ*DEG_TO_RAD) - deltaY * sin(BNO055.TiltZ*DEG_TO_RAD);
    yPos = yPos + deltaY ;//* sin(BNO055.TiltZ*DEG_TO_RAD) + deltaY * cos(BNO055.TiltZ*DEG_TO_RAD);

    lift = mouseData.isOnSurface;
    movement = mouseData.isMotion;
}

float MouseC::giveVx(){
    return deltaX;
}

float MouseC::giveVy(){
    return deltaY;
}

