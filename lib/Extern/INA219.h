#pragma once
//    FILE: INA219.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.3.1
//    DATE: 2021-05-18
// PURPOSE: Arduino library for INA219 voltage, current and power sensor
//     URL: https://github.com/RobTillaart/INA219
//
//  Read the datasheet for the details how to connect!


#include "Arduino.h"
#include "Wire.h"


#define INA219_LIB_VERSION              (F("0.3.1"))


class INA219
{
public:
  //  address between 0x40 and 0x4F
  explicit INA219(const uint8_t address);

  bool     begin();
  bool     isConnected();
  uint8_t  getAddress();


  //  CORE FUNCTIONS               //  Register
  double    getShuntVoltage();      //  01
  double    getBusVoltage();        //  02
  double    getPower();             //  03
  double    getCurrent();           //  04
  bool     getMathOverflowFlag();  //  02
  bool     getConversionFlag();    //  02


  //  Scale helpers milli range
  double    getBusVoltage_mV()   { return getBusVoltage()   * 1e3; };
  double    getShuntVoltage_mV() { return getShuntVoltage() * 1e3; };
  double    getCurrent_mA()      { return getCurrent()      * 1e3; };
  double    getPower_mW()        { return getPower()        * 1e3; };
  //  Scale helpers micro range
  double    getBusVoltage_uV()   { return getBusVoltage()   * 1e6; };
  double    getShuntVoltage_uV() { return getShuntVoltage() * 1e6; };
  double    getCurrent_uA()      { return getCurrent()      * 1e6; };
  double    getPower_uW()        { return getPower()        * 1e6; };


  //  Configuration
  //  need improvement API wise.
  bool     reset();
  //  voltage = 16, 32  (values below 32 are rounded to 16 or 32)
  bool     setBusVoltageRange(uint8_t voltage = 16);
  uint8_t  getBusVoltageRange();  //  returns 16 or 32.
  //  factor = 1, 2, 4, 8
  bool     setGain(uint8_t factor = 1);
  uint8_t  getGain();

  //  configuration BUS
  //  use one of the next three
  bool     setBusResolution(uint8_t bits);  //  9..12, always 1 sample
  bool     setBusSamples(uint8_t value);    //  0..7, always 12 bits.
  bool     setBusADC(uint8_t mask = 0x03);  //  uses a mask, check datasheet
  uint8_t  getBusADC();

  //  configuration SHUNT
  //  use one of the next three
  bool     setShuntResolution(uint8_t bits);  //  9..12, always 1 sample
  bool     setShuntSamples(uint8_t value);    //  0..7, always 12 bits.
  bool     setShuntADC(uint8_t mask = 0x03);  //  uses a mask, check datasheet
  uint8_t  getShuntADC();

  //  Calibration
  //  mandatory to set these! read datasheet.
  //  maxCurrent >= 0.001
  //  shunt      >= 0.001
  bool     setMaxCurrentShunt(double maxCurrent = 3.4,
                              double shunt = 0.002);

  bool     isCalibrated()     { return _current_LSB != 0.0; };

  //  These functions return zero if not calibrated!
  double    getCurrentLSB()    { return _current_LSB;       };
  double    getCurrentLSB_mA() { return _current_LSB * 1e3; };
  double    getCurrentLSB_uA() { return _current_LSB * 1e6; };
  double    getShunt()         { return _shunt;             };
  double    getMaxCurrent()    { return _maxCurrent;        };


  //  Operating mode = 0..7
  bool     setMode(uint8_t mode = 7);  //  default ModeShuntBusContinuous
  uint8_t  getMode();
  bool     shutDown()                  { return setMode(0); };
  bool     setModeShuntTrigger()       { return setMode(1); };
  bool     setModeBusTrigger()         { return setMode(2); };
  bool     setModeShuntBusTrigger()    { return setMode(3); };
  bool     setModeADCOff()             { return setMode(4); };
  bool     setModeShuntContinuous()    { return setMode(5); };
  bool     setModeBusContinuous()      { return setMode(6); };
  bool     setModeShuntBusContinuous() { return setMode(7); };  //  default.


  //  DEBUG
  uint16_t getRegister(uint8_t reg)  { return _readRegister(reg); };


private:

  uint16_t _readRegister(uint8_t reg);
  uint16_t _writeRegister(uint8_t reg, uint16_t value);
  double    _current_LSB;
  double    _shunt;
  double    _maxCurrent;

  uint8_t   _address;
};


//  -- END OF FILE --

