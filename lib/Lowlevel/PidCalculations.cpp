#include "PidCalculations.h"

PIDC PID;

void PIDC::Calculate(){
    now = micros();
    diffTime = double(now - last) / 1000;
    last = now;

    double error = Angle - BNO055.give_TiltZ();
    esum = esum + double(error);

    const int max_error_sum = 6000;

    if(esum > 0) // limit error sum
        esum = min(esum, max_error_sum);
    else
        esum = max(esum, -max_error_sum);

    
    if(Utils.isinRange(error, -1, 1)) {
        esum = 0;
    }
    double derivative = (error - ealt) / diffTime;
    ealt = error;

    if (error > 0){
        Out  = (error * Kp  * PID_Mult) + (esum * Ki * diffTime * PID_Mult) + (derivative * Kd);
    }else{
        Out  = (error * Kp  * PID_Mult) + -(esum * Ki * diffTime * PID_Mult) + -(derivative * Kd);
    }
    last = micros(); 
}

void PIDC::setAngle(double Angle1){
    Angle = Angle1;
}