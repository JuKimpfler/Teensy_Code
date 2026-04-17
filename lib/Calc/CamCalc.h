#pragma once

class CamCalcC{
    public:
        double BallAngle(int xIn, int yIn, double sizeX, double sizeY);
        double TorAngle(int xIn, int yIn, double sizeX, double sizeY);
};

extern CamCalcC CamCalc;