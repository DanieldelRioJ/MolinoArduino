#include "sensor_controller.h"

SensorController::SensorController(DistanceSensor *hopperSensor, DistanceSensor *containerSensor, uint8_t threshold){
  this->_hopperSensor = hopperSensor;
  this->_containerSensor = containerSensor;
  this->setContainerThreshold(threshold);
  this->reset();
}

ControlResults SensorController::getControlResults(){
  Measure hopperMeasure = this->_hopperSensor->getDistance();
  Measure containerMeasure = this->_containerSensor->getDistance();
  ControlResults results;

  results.isHopperEmpty = false;
  if(hopperMeasure.status != VL6180X_ERROR_NONE){
    hopperMeasure.range = 250;
  }
  this->_hopperMeanMeasure = 0.9 * this->_hopperMeanMeasure + 0.1 * hopperMeasure.range; //mm
  //Serial.println(String(hopperMeasure.range) +", "+ String(this->_hopperMeanMeasure));
  if(this->_hopperMeanMeasure > DEFAULT_HOPPER_THRESHOLD){    
    if(this->_timesCheckedHopperEmpty > MAX_EXCEEDED_MEASURES_BEFORE_WARNING){
      results.isHopperEmpty = true;
    }else{      
      this->_timesCheckedHopperEmpty++;
    }
  }else{
    this->_timesCheckedHopperEmpty = 0;
  }

  results.isContainerFull = false;
  if(containerMeasure.status != VL6180X_ERROR_NONE){
    containerMeasure.range = 250;
  }
  this->_containerMeanMeasure = 0.9 * this->_containerMeanMeasure + 0.1 * containerMeasure.range; //mm
  //Serial.println(String(containerMeasure.range) +", "+ String(this->_containerMeanMeasure) + ", " + String(this->_containerThreshold) + ", "+ this->_timesCheckedContainerFull);
  if(this->_containerMeanMeasure < this->_containerThreshold){
    if(this->_timesCheckedContainerFull > MAX_EXCEEDED_MEASURES_BEFORE_WARNING){
      results.isContainerFull = true;
    }else{      
      this->_timesCheckedContainerFull++;
    }
  }else{
    this->_timesCheckedContainerFull = 0;
  }

  return results;
}

void SensorController::setContainerThreshold(float threshold){
  this->_containerThreshold = threshold;
}

void SensorController::reset(){
  this->_hopperMeanMeasure = 0;
  this->_containerMeanMeasure = 255;
  this->_timesCheckedHopperEmpty = 0;
  this->_timesCheckedContainerFull = 0;
}
