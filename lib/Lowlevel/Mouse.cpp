#include "Mouse.h"
#include "BNO055.h"

MouseC Mouse;
PMW3389 pmw = PMW3389(&SPI, 24); // SPI CS vom Moussensor kommt auf PixyTx

void MouseC::init(){
    delay(1000);
    pmw.begin();
    delay(50);
    //pmwId = pmw.getProductID();
}

void MouseC::read(){
    PMW3389_DATA mouseData = pmw.readBurst();

    deltaX = -1 * mouseData.dx * 1.25 / 100;
    deltaY = mouseData.dy * 1.25 / 100;

    delta_dist = sqrtf(pow(deltaY,2)+pow(deltaX,2));

    xPos = xPos + deltaX ;//* cos(BNO055.give_TiltZ()*DEG_TO_RAD) - deltaY * sin(BNO055.give_TiltZ()*DEG_TO_RAD);
    yPos = yPos + deltaX ;//* sin(BNO055.give_TiltZ()*DEG_TO_RAD) + deltaY * cos(BNO055.give_TiltZ()*DEG_TO_RAD);

    lift = mouseData.isOnSurface;
    movement = mouseData.isMotion;
}

