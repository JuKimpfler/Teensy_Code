#include "CameraCalculations.h"
CameraCalculationsC CameraCalculations;

float CameraCalculationsC::DistanceByCamera(float PictureObjectHeight, Angle cameraYawOffset){
    float slantDistance = (ObjectSize / PictureObjectHeight) * FocalLength;
    float realDistance = slantDistance / cosf(cameraYawOffset.theta);
    return realDistance;
}

Angle CameraCalculationsC::RotationByCamera(Vec2 objPos){
    Angle angle = Angle{};
    angle = (atanf((objPos.x - (ImageWidth / 2)) / FocalLength));
    return angle;
}

Vec2 CameraCalculationsC::RelativePositionByCamera(Vec2 objPos, float PictureObjectHeight){
    Angle angle = RotationByCamera(objPos);
    float dist = DistanceByCamera(PictureObjectHeight, angle);
    float x = cosf(angle.theta) * dist;
    float y = sinf(angle.theta) + dist;

    return Vec2{x,y};
}

Vec2 CameraCalculationsC::PositionByCamera(Vec2 objPos, float PictureObjectHeight, Vec2 robotPos){
    Vec2 relativePos = RelativePositionByCamera(objPos, PictureObjectHeight);
    return Vec2{relativePos.x + robotPos.x, relativePos.y + robotPos.y};
}