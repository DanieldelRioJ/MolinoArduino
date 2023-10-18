#ifndef DistanceSensor_h
#define DistanceSensor_h
#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_VL6180X.h>

struct Measure{
  uint8_t range;
  uint8_t status;
};

class DistanceSensor
{
  public:
    DistanceSensor(uint8_t shtdwnPin);
    boolean begin();
    void onOff(bool onOff);
    void setAddress(uint8_t newAddr);
    Measure getDistance();

  private:
    uint8_t address;
    uint8_t shtdwnPin;
    Adafruit_VL6180X sensor;

};

#endif