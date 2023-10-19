#include "distance_sensor.h"

DistanceSensor::DistanceSensor(uint8_t shtdwnPin){
  pinMode(shtdwnPin, OUTPUT);
  this->sensor = Adafruit_VL6180X();
  this->shtdwnPin = shtdwnPin;
  this->onOff(false);
}

boolean DistanceSensor::begin(){
  return this->sensor.begin();
}

void DistanceSensor::onOff(bool onOff){
  digitalWrite(this->shtdwnPin, onOff? HIGH: LOW);
}

void DistanceSensor::setAddress(uint8_t newAddr){
  this->address = newAddr;
  this->sensor.setAddress(this->address);
}

Measure DistanceSensor::getDistance(){
  uint8_t range = this->sensor.readRange();
  uint8_t status = this->sensor.readRangeStatus();

  // Some error occurred, print it out!

  /*if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.print("(System error)");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.print("(ECE failure)");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.print("(No convergence)");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.print("(Ignoring range)");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.print("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.print("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.print("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.print("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.print("Range reading overflow");
  }*/

  Measure measure;
  measure.range = range;
  measure.status = status;
  return measure;
}

