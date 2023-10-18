#include "alarm.h"
#include "distance_sensor.h"

#define ALARM_PIN 12
#define HOPPER_DISTANCE_SENSOR_SHUTDOWN_PIN 11
#define CONTAINER_DISTANCE_SENSOR_SHUTDOWN_PIN 10

#define HOPPER_DISTANCE_SENSOR_ADDRESS 0x30
#define CONTAINER_DISTANCE_SENSOR_ADDRESS 0x30

Alarm al(ALARM_PIN);
/*DistanceSensor hopperDistanceSensor(HOPPER_DISTANCE_SENSOR_SHUTDOWN_PIN);
DistanceSensor containerDistanceSensor(CONTAINER_DISTANCE_SENSOR_SHUTDOWN_PIN);
bool firstLoop = true;*/

void setup() {
  Serial.begin(9600);
  /*hopperDistanceSensor.onOff(false);
  delay(1000);
  hopperDistanceSensor.onOff(true);
  delay(10);
  if(!hopperDistanceSensor.begin()){
    Serial.println("Failed to boot HopperSensor");
    while(1);
  }
  hopperDistanceSensor.setAddress(HOPPER_DISTANCE_SENSOR_ADDRESS);
  delay(10);*/


  /*if(!containerDistanceSensor.begin()){
    Serial.println("Failed to boot DistanceSensor");
    while(1);
  }
  containerDistanceSensor.setAddress(CONTAINER_DISTANCE_SENSOR_ADDRESS);
  delay(10);*/
  al.mode(AlarmMode::short_ringing);
}

void loop(){
  al.loop();
  //const Measure measure = hopperDistanceSensor.getDistance();
}