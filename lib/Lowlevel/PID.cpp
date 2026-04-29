#include "PID.h"
PIDC PID;

void PIDC::Calculate(){
    now = micros();
    diffTime = (now - last) / 1000;
    last = now;

    float error=0;

    error = Angle - BNO055.TiltZ;

    if (error > 180) error -= 360;
    if (error < -180) error += 360;
    esum = esum + float(error);

    const int max_error_sum = 6000;
    if(esum > 0)
        esum = min(esum, max_error_sum);
    else
        esum = max(esum, -max_error_sum);

    if(U.Ran(error, -1, 1)) {
        esum = 0;
    }
    float derivative ;//= (error - ealt) / diffTime;
    float SchwelleD = 3;
    if(abs(error) < SchwelleD){
        if((error - ealt) == 0.0){
            derivative = (error - ealt);
        }
        else{
            derivative = (error - ealt) / (sqrtf(abs(error)) * diffTime);
        }
        Out = (error * Kp * PID_Mult) + (esum * Ki * diffTime * PID_Mult) + (derivative * Kd);
    }
    else if(abs(error) < SchwelleD+2){
        if(abs(error) < SchwelleD){
            if((error - ealt) == 0.0){
                derivative = (error - ealt);
            }
            else{
                derivative = (error - ealt) / (sqrtf(abs(error)) * diffTime);
            }
            Out = (error * Kp * PID_Mult) + (esum * Ki * diffTime * PID_Mult) + (derivative * Kd);
        }
    }
    else if(abs(error) >= SchwelleD+2){
        if((error - ealt) == 0.0){
            derivative = (error - ealt);
        }
        else{
            derivative = (error - ealt) / (sqrtf(abs(error)) * diffTime);
        }
        Out = (error * Kp * PID_Mult) + (esum * Ki * diffTime * PID_Mult) + (derivative * Kd);
    }
    else{
        derivative = (error - ealt) / (sqrtf(abs(SchwelleD)) * diffTime);
        Out = (error * Kp * PID_Mult) + (esum * Ki * diffTime * PID_Mult) + (derivative * Kd);
    }
    ealt = error;

    last = micros(); 
}

void PIDC::setAngle(float Angle1){
    Angle = Angle1;
}