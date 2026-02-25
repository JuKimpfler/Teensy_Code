#include "PidCalculations.h"

PIDC PID;

void PIDC::Calculate(){
    now = micros();
    diffTime = float(now - last) / 1000;
    last = now;

    float error = Angle - BNO055.TiltZ;
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
    float derivative = (error - ealt) / diffTime;
    ealt = error;

    // Korrekte PID-Berechnung ohne abs() und ohne Vorzeichenwechsel!
    Out = (error * Kp * PID_Mult) + (esum * Ki * diffTime * PID_Mult) + (derivative * Kd);

    last = micros(); 
}

void PIDC::setAngle(float Angle1){
    Angle = Angle1;
}