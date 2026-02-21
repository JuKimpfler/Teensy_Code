#include "BNO055.h"
#include "RGB.h"

Adafruit_BNO055 bno(55, 0x28, &Wire1);

BNO055C BNO055;

/**
 * Initializes the BNO055 sensor. This function calls begin() on the BNO055
 * object and sets the external crystal to be used.
 */
void BNO055C::init(){
    bno.begin();
    bno.setExtCrystalUse(true);
}


/**
 * Reads data from the BNO055 sensor and calculates the tilt angles in degrees.
 * The tilt angles are calculated based on the orientation data from the sensor.
 * The angles are then normalized to be within the range of -180 to 180 degrees.
 * The calculated angles are stored in the class variables TiltX, TiltY and TiltZ.
 * These variables can then be accessed using the getter methods.
 */

void BNO055C::read(){
    sensors_event_t event; 
    bno.getEvent(&event);

    // Berechnung für TiltZ (Rotation um Z-Achse)
    double Comp_Dir = event.orientation.x;
    Comp_Dir = Comp_Dir-BNO_Cal;
    if (Comp_Dir<0){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    if(Comp_Dir<=180){Comp_Dir=Comp_Dir;}
    else if (Comp_Dir>180){Comp_Dir=Comp_Dir-360;}
    if (Comp_Dir<-180){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    TiltZ = Comp_Dir;

    // Berechnung für TiltX (Rotation um X-Achse)
    Comp_Dir = event.orientation.y;
    if (Comp_Dir<0){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    if(Comp_Dir<=180){Comp_Dir=Comp_Dir;}
    else if (Comp_Dir>180){Comp_Dir=Comp_Dir-360;}
    if (Comp_Dir<-180){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    TiltX = Comp_Dir;

    // Berechnung für TiltY (Rotation um Y-Achse)
    Comp_Dir = event.orientation.z;
    if (Comp_Dir<0){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    if(Comp_Dir<=180){Comp_Dir=Comp_Dir;}
    else if (Comp_Dir>180){Comp_Dir=Comp_Dir-360;}
    if (Comp_Dir<-180){Comp_Dir = Comp_Dir+360;}
    else if (Comp_Dir>359){Comp_Dir = Comp_Dir-360;}
    TiltY = Comp_Dir;
}

/**
 * Calibrates the BNO055 sensor. This function reads the current orientation from the sensor
 * and sets the calibration register to the current value of the x-axis orientation.
 * This value is then used as the reference for the tilt angles calculations.
 * The calibration register is stored in the class variable BNO_Cal.
 */
void BNO055C::Calibrate(){
    sensors_event_t event; 
    bno.getEvent(&event);
    BNO_Cal = event.orientation.x;
}


/**
 * Shows the calibration status of the BNO055 sensor on the first RGB LED.
 * The color of the LED is determined by the calibration status of the sensor.
 * If the magnetometer is not calibrated, the LED will be red.
 * If the magnetometer is calibrated but not fully, the LED will be orange.
 * If the magnetometer is fully calibrated, the LED will be yellow.
 * If the magnetometer is fully calibrated and the sensor is fully calibrated, the LED will be green.
 */
void BNO055C::showCal(){
    uint8_t system2 = 0;
    uint8_t gyro = 0;
    uint8_t accel = 0;
    uint8_t mag = 0;
    bno.getCalibration(&system2, &gyro, &accel, &mag);

    if (mag==0){RGB.write(0,"R");}// LED AN
    else if (mag==1){RGB.write(0,"O");}// LED AN
    else if (mag==2){RGB.write(0,"Y");}// LED AN
    else if (mag==3){RGB.write(0,"G");}// LED AN
}



/**
 * Returns the tilt angle around the x-axis in degrees.
 * This value is calculated based on the calibration register and the current orientation of the sensor.
 * @return The tilt angle around the x-axis in degrees.
 */
double BNO055C::give_TiltX(){
    return TiltX;
}
/**
 * Returns the tilt angle around the y-axis in degrees.
 * This value is calculated based on the calibration register and the current orientation of the sensor.
 * @return The tilt angle around the y-axis in degrees.
 */

/**
 * Returns the tilt angle around the y-axis in degrees.
 * This value is calculated based on the calibration register and the current orientation of the sensor.
 * @return The tilt angle around the y-axis in degrees.
 */
double BNO055C::give_TiltY(){
    return TiltY;
}

