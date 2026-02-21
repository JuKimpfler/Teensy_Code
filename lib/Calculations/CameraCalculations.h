#pragma once
#include <structs.h>
#include <Param.h>
#include <math.h>

class CameraCalculationsC{
    public:
        float DistanceByCamera(float PictureObjectHeight, Angle cameraYawOffset);
        Angle RotationByCamera(Vec2 ballPos);
        Vec2 RelativePositionByCamera(Vec2 ballPos, float PictureObjectHeight);
        Vec2 PositionByCamera(Vec2 ballPos, float PictureOffsetHeight, Vec2 RobotPos);
};

extern CameraCalculationsC CameraCalculations;