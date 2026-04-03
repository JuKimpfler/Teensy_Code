#pragma once
#include "Elementar.h"

class CamC{
    private:
        void readSerialFromOpenMV();
    public:
        bool readCameraAbsolute(float &camX, float &camY);

        float last_blob_cx = 160.0;
        float last_blob_w = 0.0;
        char last_goal_color = 'N'; 
        unsigned long last_cam_update = 0;
};

extern CamC Cam;

class GoalC{
    public:
        double Angle;
        double Distance;
        double X;
        double Y;
        double Area;
        bool inSight;
        bool Regeln;
        bool lastdir; // true = - / false = +
    private:
};

extern GoalC Goal;
 