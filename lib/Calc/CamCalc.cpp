#include "CamCalc.h"

double CamCalcC::BallAngle(int xIn, int yIn, double sizeX, double sizeY){
    double fov = 120;
    int pxOnScreen = 1800;
    double angle = (1-(xIn/pxOnScreen))*(fov-fov/2);
    return angle;
}

double CamCalcC::TorAngle(int xIn, int yIn, double sizeX, double sizeY){
    double fov = 120;
    int pxOnScreen = 1800;
    double angle = (1-(xIn/pxOnScreen))*(fov-fov/2);
    return angle;
}