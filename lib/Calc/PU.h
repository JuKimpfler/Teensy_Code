#pragma once
#include <Arduino.h>
#include <math.h>
#include "Elementar.h"

struct Pose2D {
  float x_cm;
  float y_cm;
  float Speed;
  bool  valid;
};

class PUC{
    public:
        Pose2D Positon;

        float AlongBorder(
            float x, float y, float desiredDeg,
            float marginCm,
            float lookaheadCm
        );

        void update(
            float yawDeg,                 // globale Drehung (Z) der Kamera-Einheit
            float angle1Deg, float h1Px, bool vis1,  // Cam1 (z.B. GELB)
            float angle2Deg, float h2Px, bool vis2,  // Cam2 (z.B. BLAU)
            float fy_px = 220.0f,         // STARTWERT! kalibrieren!
            float FIELD_W = 182.0f,
            float FIELD_L = 243.0f
        );
};

extern PUC PU; // PositionUnit per 2 OpenMV R1 back to back