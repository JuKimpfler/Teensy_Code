#include "CamCalc.h"
CamCalcC CamCalc;

/*double CamCalcC::DistByCam(double PictureObjectHeight, Angle cameraYawOffset){
    double slantDistance = (ObjectSize / PictureObjectHeight) * FocalLength;
    double realDistance = slantDistance / cosf(cameraYawOffset.theta);
    return realDistance;
}

Angle CamCalcC::RotationByCam(Vec2 objPos){
    Angle angle = Angle{};
    angle = (atanf((objPos.x - (ImageWidth / 2)) / FocalLength));
    return angle;
}

Vec2 CamCalcC::RelaPosByCam(Vec2 objPos, double PictureObjectHeight){
    Angle angle = RotationByCam(objPos);
    double dist = DistByCam(PictureObjectHeight, angle);
    double x = cosf(angle.theta) * dist;
    double y = sinf(angle.theta) + dist;

    return Vec2{x,y};
}

Vec2 CamCalcC::PosByCam(Vec2 objPos, double PictureObjectHeight, Vec2 robotPos){
    Vec2 relativePos = RelaPosByCam(objPos, PictureObjectHeight);
    return Vec2{relativePos.x + robotPos.x, relativePos.y + robotPos.y};
}*/

void CamCalcC::Angle(){
    Goal.Angle ;
}