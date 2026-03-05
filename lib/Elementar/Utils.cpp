#include "Utils.h"
UtilsC U;
bool UtilsC::Ran(float In,float min, float max){
    if(min < max){
        return In >= min && In <= max;
    }
    else{
        return In <= min && In >= max;
    }
};

double UtilsC::Circel(double Angle){
    if(Angle<-180){Angle = Angle+ 360;}
    else if (Angle > 180){Angle = Angle - 360;}
    return Angle;
}

uint16_t* UtilsC::List_copy(const uint16_t source[], uint16_t dest[], int len) {
    for (int i = 0; i < len; i++) {
        dest[i] = source[i];
    }
    return dest;
}

int16_t* UtilsC::List_copy(const int16_t source[], int16_t dest[], int len) {
    for (int i = 0; i < len; i++) {
        dest[i] = source[i];
    }
    return dest;
}
