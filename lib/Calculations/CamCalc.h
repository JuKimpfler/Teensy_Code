#pragma once
#include <Param.h>
#include <math.h>
#include "IR.h"
#include "Cam.h"

class CamCalcC{
    public:
        /*float DistByCam(float PictureObjectHeight, Angle cameraYawOffset);
        Angle RotationByCam(Vec2 ballPos);
        Vec2 RelaPosByCam(Vec2 ballPos, float PictureObjectHeight);
        Vec2 PosByCam(Vec2 ballPos, float PictureOffsetHeight, Vec2 RobotPos);*/
        void Angle();
};

extern CamCalcC CamCalc;