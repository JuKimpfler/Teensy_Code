
#include "Utils.h"
UtilsC U;
bool UtilsC::Ran(float In,float min, float max){
    if (In < max && In > min){
        return true;
    }
    else {return false;}
};
