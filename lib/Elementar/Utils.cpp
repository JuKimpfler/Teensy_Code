
#include "Utils.h"
UtilsC U;
bool UtilsC::Ran(float In,float min, float max){
    if (In < max && In > min){
        return true;
    }
    else {return false;}
};

double UtilsC::Circel(double Angle){
    if(Angle<-180){Angle = Angle+ 360;}
    else if (Angle > 180){Angle = Angle - 360;}
    return Angle;
}

/*int16_t UtilsC::Circel(int16_t Angle){
    if(Angle<-180){Angle = Angle+ 360;}
    else if (Angle > 180){Angle = Angle - 360;}
    return Angle;
}*/
