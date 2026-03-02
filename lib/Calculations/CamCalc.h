#pragma once
#include <Param.h>
#include <math.h>
#include "IR.h"
#include "Cam.h"

class CamCalcC{
    public:
        /*double DistByCam(double PictureObjectHeight, Angle cameraYawOffset);
        Angle RotationByCam(Vec2 ballPos);
        Vec2 RelaPosByCam(Vec2 ballPos, double PictureObjectHeight);
        Vec2 PosByCam(Vec2 ballPos, double PictureOffsetHeight, Vec2 RobotPos);*/
        void Angle();
};

extern CamCalcC CamCalc;