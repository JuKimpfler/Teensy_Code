#include "BNO055_beaver.h"
#include "EEPROM.h"

// ? was bringt das
//#include <vector>

// Initialize BNO055
void BNO055_beaver::init() {
  // BNO055_beaver in Konfigurationsmodus bringen
  cls->beginTransmission(this->addr);
  cls->write(REG_OPR_MODE);
  cls->write(OPR_MODE_CONFIGMODE);
  cls->endTransmission();

  delayMicroseconds(50);

#ifdef CALIBRATION_ENABLED
  // Calibration
  __disable_irq();
  eeprom_read_block((void*) calibDataByte, (void*) EEPROM_BNO_CALIB, EEPROM_BNO_CALIB_SIZE);
  __enable_irq();

  uint8_t otherThanZero = 0;
  for (uint8_t i = 0; i < 11; i++) {
    calibData[i] = calibDataByte[i << 1] | calibDataByte[(i << 1) + 1] << 8;
    if (calibDataByte[i << 1] || calibDataByte[(i << 1) + 1]) {
      otherThanZero = 1;
    }
  }

  if (otherThanZero) {
    for (uint8_t i = 0; i < 22; i++) {
      cls->beginTransmission(this->addr);
      cls->write(0x55 + i);
      cls->write(calibDataByte[i]);
      cls->endTransmission();
      delayMicroseconds(10);
    }
  }
#endif

  // Puffer für Datentransfer
  uint8_t buffer[2] = {0, 0};

  // Unit Selection
  cls->beginTransmission(addr);
  buffer[0] = REG_UNIT_SEL;
  // Einheiten festlegen (BNO055_beaver.pdf S.69)
  // Bit7: Data output format = 1 -> Android (verändert Ausgabe von Pitch-Werten)
  // Bit4: Temperature = 0 -> Celsius
  // Bit2: Euler units = 0 -> Grad (Winkeleinheit)
  // Bit1: Angular Rate units = 0 -> Dps (Rotationsgeschwindigkeit Grad/s)
  // Bit0: Acceleration units = 0 -> m/s² (Beschleunigung)
  buffer[1] = (1 << 7) | (0 << 4) | (0 << 2) | (0 << 1) | (1 << 0);
  cls->write(buffer, 2);

  // TODO: evtl neue Transmission starten

  // BNO055_beaver in Standardmodus bringen
  buffer[0] = REG_OPR_MODE;
  buffer[1] = DEFAULT_OPR_MODE;
  cls->write(buffer[0]);
  cls->write(buffer[1]);
  // ? warum nicht: cls->write(buffer, 2);

  cls->endTransmission();
}

// mehrere Words auslesen
inline void BNO055_beaver::getWords(uint8_t start_address, int16_t* data, uint8_t numWords) {
  cls->beginTransmission(this->addr);
  cls->write(start_address);
  cls->endTransmission();
  cls->requestFrom(this->addr, (size_t) (numWords * 2));
  uint8_t bytes[numWords * 2];
  if (cls->available() != 8) {
    memset((void*) data, 0, numWords);
    return;
  }
  cls->readBytes(bytes, (size_t) (numWords * 2));

  for (uint8_t i = 0; i < numWords; i++) {
    data[i] = (int16_t) ((int16_t) (bytes[1 + i * 2]) << 8 | bytes[0 + i * 2]);
  }
}

// Word auslesen
inline int16_t BNO055_beaver::getWord(uint8_t address) {
  cls->requestFrom((int) this->addr, 2);
  uint8_t ret[2];
  cls->readBytes(ret, 2);

  return (int16_t) (((int16_t) (ret[1]) << 8) | ret[0]);
}

// Reset BNO055_beaver System
void BNO055_beaver::reset() {
#ifdef CALIBRATION_ENABLED
  memset(this->calibDataByte, 0, 22);
  __disable_irq();
  eeprom_write_block((void*) calibDataByte, (void*) EEPROM_BNO_CALIB, EEPROM_BNO_CALIB_SIZE);
  __enable_irq();
#endif

  // Reset System
  uint8_t buffer[2] = {REG_SYS_TRIGGER, (1 << 5)};
  cls->beginTransmission(this->addr);
  cls->write(buffer[0]);
  cls->write(buffer[1]);
  // cls->write(buffer, 2);
  cls->endTransmission();
}

// Get System Error
uint8_t BNO055_beaver::systemError() {
  cls->beginTransmission(addr);
  cls->write(REG_SYS_ERR);
  cls->endTransmission();
  cls->requestFrom((int) addr, 1);
  return (uint8_t) (cls->read());
}

// Get Calibration Status
uint8_t BNO055_beaver::calibStatus() {
  cls->beginTransmission(addr);
  cls->write(REG_CALIB_STAT);
  cls->endTransmission();
  cls->requestFrom((int) addr, 1);
  return (uint8_t) (cls->read());
}

#ifdef CALIBRATION_ENABLED
void BNO055_beaver::updateCalibrationData() {
  // BNO055_beaver in Konfigurationsmodus setzen
  cls->beginTransmission(this->addr);
  cls->write(REG_OPR_MODE);
  cls->write(OPR_MODE_CONFIGMODE);
  cls->endTransmission();

  delay(30);

  // Calib Data auslesen
  cls->beginTransmission(this->addr);
  cls->write(0x55);   // ACC_OFFSET_X_LSB unterster Byte
  cls->endTransmission();
  cls->requestFrom((int) this->addr, 22);

  cls->readBytes(calibDataByte, 22);
  for (uint8_t i = 0; i < 11; i++) {
    calibData[i] = (calibDataByte[i << 1] | (calibDataByte[(i << 1) + 1] << 8));
  }

  cls->beginTransmission(this->addr);
  cls->write(0x3D);
  cls->write(DEFAULT_OPR_MODE);
  cls->endTransmission();
  delay(30);

  __disable_irq();
  eeprom_write_block((void*) calibDataByte, (void*) EEPROM_BNO_CALIB, EEPROM_BNO_CALIB_SIZE);
  __enable_irq();
}
#endif

// Get System Status
e_bnoSysStatus BNO055_beaver::systemStatus() {
  cls->beginTransmission(this->addr);
  cls->write(REG_SYS_STATUS);
  cls->endTransmission();
  cls->requestFrom((int) this->addr, 1);
  return (e_bnoSysStatus) cls->read();
}

uint8_t BNO055_beaver::requestSystemStatus() {
  cls->beginTransmission(this->addr);
  cls->write(0x39);
  cls->endTransmission();
  cls->requestFrom((int) this->addr, 1);
  return 1;
}

// Ausrichtung in Quaternionen
void BNO055_beaver::quaData(float* data) {
  int16_t quat[4];
  getWords(0x20, quat, 4);   // unterster quaternion byte
  for (uint8_t i = 0; i < 4; i++) {
    data[i] = quat[i] / (float) (1 << 14);
  }
}

uint8_t BNO055_beaver::requestQuaData() {
  cls->beginTransmission(this->addr);
  cls->write(0x20);
  cls->endTransmission();
  cls->requestFrom(this->addr, (size_t) (8));
  return 1;
}

float BNO055_beaver::quaDataW() {
  return (float) (this->getWord(0x20) / ((float) (1 << 14)));
}

float BNO055_beaver::quaDataX() {
  return (float) (this->getWord(0x22) / ((float) (1 << 14)));
}

float BNO055_beaver::quaDataY() {
  return (float) (this->getWord(0x24) / ((float) (1 << 14)));
}

float BNO055_beaver::quaDataZ() {
  return (float) (this->getWord(0x26) / ((float) (1 << 14)));
}

//Beschleunigungs Vektoren
int16_t BNO055_beaver::accDataX(void) {
  return this->getWord(0x8);
}

int16_t BNO055_beaver::accDataY(void) {
  return this->getWord(0xA);
}

int16_t BNO055_beaver::accDataZ(void) {
  return this->getWord(0xC);
}

//Magnetfeld Vektoren
int16_t BNO055_beaver::magDataX(void) {
  return this->getWord(0xE);
}

int16_t BNO055_beaver::magDataY(void) {
  return this->getWord(0x10);
}

int16_t BNO055_beaver::magDataZ(void) {
  return this->getWord(0x12);
}

//Gyro Vektoren
int16_t BNO055_beaver::gyrDataX(void) {
  return this->getWord(0x14) / 10.0f;
}

int16_t BNO055_beaver::gyrDataY(void) {
  return this->getWord(0x16) / 10.0f;
}

int16_t BNO055_beaver::gyrDataZ(void) {
  return this->getWord(0x18) / 10.0f;
}

uint8_t BNO055_beaver::requestGyrDataZ() {
  cls->beginTransmission(this->addr);
  cls->write(0x18);
  cls->endTransmission();
  cls->requestFrom((int) this->addr, 2);
  return 1;
}

//Ausrichtung in Euler-Winkeln
float BNO055_beaver::eulHeading(void) {
  return (float) (this->getWord(0x1A) / 16.0f);
}

float BNO055_beaver::eulPitch(void) {
  return (int16_t) (this->getWord(0x1E) / 16.0f);
}

float BNO055_beaver::eulRoll(void) {
  return (float) (this->getWord(0x1C) / 16.0f);
}
