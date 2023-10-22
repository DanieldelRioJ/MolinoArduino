#include "distance_dial.h"
#include "Arduino.h"

DistanceDial::DistanceDial(uint8_t pin){
  this->_pin = pin;  
  pinMode(this->_pin, INPUT);
};

float DistanceDial::getDistance(){  
  return MAX_DISTANCE - float(analogRead(this->_pin)) * (MAX_DISTANCE - MIN_DISTANCE) / 1024; //Max 15cm, min 5cm
}