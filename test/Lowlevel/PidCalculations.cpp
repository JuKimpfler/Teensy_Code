#include "PidCalculations.h"

PIDC PID;

void PIDC::Calculate(){
    now = micros();
    diffTime = float(now - last) / 1000;
    last = now;

    float error = Angle - BNO055.give_TiltZ();
    esum = esum + float(error);

    const int max_error_sum = 6000;

    if(esum > 0) // limit error sum
        esum = min(esum, max_error_sum);
    else
        esum = max(esum, -max_error_sum);

    
    if(Utils.isinRange(error, -1, 1)) {
        esum = 0;
    }
    float derivative = (error - ealt) / diffTime;
    ealt = error;

    if (error > 0){
        Out  = (error * Kp ) + -(abs(esum * Ki * diffTime)) + abs(derivative * Kd);
    }else{
        Out  = (error * Kp ) + (abs(esum * Ki * diffTime)) + -(abs(derivative * Kd));
    }
    last = micros(); 
    Serial.println(diffTime);
}

void PIDC::setAngle(float Angle1){
    Angle = Angle1;
}