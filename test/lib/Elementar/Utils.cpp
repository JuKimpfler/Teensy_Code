
#include "Utils.h"
UtilsC Utils;
bool UtilsC::isinRange(double In,double min, double max){
    if (In < max && In > min){
        return true;
    }
    else {return false;}
};
