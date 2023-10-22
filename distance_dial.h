#ifndef DistanceDial_h
#define DistanceDial_h


#include "Arduino.h"
#define MAX_DISTANCE 150
#define MIN_DISTANCE 50

class DistanceDial
{
  public:
    DistanceDial(uint8_t pin);
    float getDistance();

  private:
    uint8_t _pin;
};

#endif