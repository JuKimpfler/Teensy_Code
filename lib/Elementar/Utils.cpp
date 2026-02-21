
#include "Utils.h"
UtilsC Utils;
bool UtilsC::isinRange(float In,float min, float max){
    if (In < max && In > min){
        return true;
    }
    else {return false;}
};
