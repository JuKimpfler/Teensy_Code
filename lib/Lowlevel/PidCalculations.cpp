#include "PidCalculations.h"
#include "Cam.h"
PIDC PID;

void PIDC::Calculate(){
    now = micros();
    diffTime = (now - last) / 1000;
    last = now;

    float error=0;

    if(Goal.Regeln == true){
        error = Angle - Goal.Angle;
    }
    else{
        error = Angle - BNO055.TiltZ;
    }
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
    float SchwelleD = 1;
    if(error >= SchwelleD){
        if((error - ealt) == 0.0){
            derivative = (error - ealt);
        }
        else{
            derivative = (error - ealt) / (sqrtf(error) * diffTime);
        }
    }
    else{
        derivative = (error - ealt) / (sqrtf(SchwelleD) * diffTime);
    }
    ealt = error;

    Out = (error * Kp * PID_Mult) + (esum * Ki * diffTime * PID_Mult) + (derivative * Kd);

    last = micros(); 
}

void PIDC::setAngle(float Angle1){
    Angle = Angle1;
}