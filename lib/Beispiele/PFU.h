#pragma once
#include <Arduino.h>
#include <math.h>
#include "Cam.h"
#include "US.h"
#include "Mouse.h"
#include "BNO055.h"
#include "Elementar.h"
#include "Line.h"

class Kalmanfilter{
    private:
        int X;
        int Y;
        int GAngle;
        int MaxSpeed;
        int HomeAngle;
        int BallX;
        int BallY;

    public:
        void begin();
        void Update();
        void updateOdometry(float dt);
        void updateUltrasonic();
        void checkLineSensors();

        int giveX();
        int giveY();
        int giveGAngle();
        int giveSpeed();
        int giveHomeAngle();
        int giveBallX();
        int giveBallY();

        // --- Wände (Ultraschall) ---
        const float WALL_X_LEFT   = 0.0;
        const float WALL_X_RIGHT  = 182.0;
        const float WALL_Y_BOTTOM = 0.0;
        const float WALL_Y_TOP    = 243.0;

        // --- Linien (Liniensensoren) ---
        const int NUM_X_LINES = 4;
        const float LINES_X[4] = {10.0, 61.0, 121.0, 172.0};

        const int NUM_Y_LINES = 4;
        const float LINES_Y[4] = {10.0, 35.0, 208.0, 233.0};

        // --- Roboter Geometrie & Filter ---
        const float R_SENSOR = 15.0;            
        const int NUM_LINE_SENSORS = 32;
        float sensorAngles[32];   

        const float K_CAM = 0.2f;               
        const float K_US = 0.05f;               
        const float MAX_DRIFT_TOLERANCE = 40.0; 

        // --- Zustandsvariablen ---
        volatile float posX = 91.0;   
        volatile float posY = 121.5;  
        volatile float thetaDeg = 0.0;
        volatile float thetaRad = 0.0;

        unsigned long last500Hz = 0;
        unsigned long last20Hz = 0;
        unsigned long last10Hz = 0;

};

extern Kalmanfilter PFU; // PositionFilteringUnit