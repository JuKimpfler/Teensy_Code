#include "CamCalc.h"
CamCalcC CamCalc;

/*float CamCalcC::DistByCam(float PictureObjectHeight, Angle cameraYawOffset){
    float slantDistance = (ObjectSize / PictureObjectHeight) * FocalLength;
    float realDistance = slantDistance / cosf(cameraYawOffset.theta);
    return realDistance;
}

Angle CamCalcC::RotationByCam(Vec2 objPos){
    Angle angle = Angle{};
    angle = (atanf((objPos.x - (ImageWidth / 2)) / FocalLength));
    return angle;
}

Vec2 CamCalcC::RelaPosByCam(Vec2 objPos, float PictureObjectHeight){
    Angle angle = RotationByCam(objPos);
    float dist = DistByCam(PictureObjectHeight, angle);
    float x = cosf(angle.theta) * dist;
    float y = sinf(angle.theta) + dist;

    return Vec2{x,y};
}

Vec2 CamCalcC::PosByCam(Vec2 objPos, float PictureObjectHeight, Vec2 robotPos){
    Vec2 relativePos = RelaPosByCam(objPos, PictureObjectHeight);
    return Vec2{relativePos.x + robotPos.x, relativePos.y + robotPos.y};
}*/

void CamCalcC::Angle(){
    Goal.Angle ;
}