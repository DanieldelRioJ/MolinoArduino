#ifndef SensorController_h
#define SensorController_h
#include "Arduino.h"
#include "Wire.h"
#include "distance_sensor.h"
#include <Adafruit_VL6180X.h>

#define DEFAULT_CONTAINER_THRESHOLD 200 //mm
#define DEFAULT_HOPPER_THRESHOLD 30 //mm

#define MAX_EXCEEDED_MEASURES_BEFORE_WARNING 20


struct ControlResults{
  bool isContainerFull;
  bool isHopperEmpty;
};

class SensorController
{
  public:
    SensorController(DistanceSensor *hopperSensor, DistanceSensor *containerSensor, uint8_t threshold = DEFAULT_CONTAINER_THRESHOLD);
    ControlResults getControlResults();    
    void setContainerThreshold(float threshold);
    void reset();

  private:
    DistanceSensor *_hopperSensor;
    DistanceSensor *_containerSensor;
    float _hopperMeanMeasure;
    float _containerMeanMeasure;
    uint8_t _timesCheckedHopperEmpty;
    uint8_t _timesCheckedContainerFull;
    uint8_t _containerThreshold;
};

#endif