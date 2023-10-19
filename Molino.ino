#include "blinker.h"
#include "distance_sensor.h"

#define BUZZER 2
#define DISTANCE_DIAL A1
#define RESET_BUTTON 5

#define HOPPER_DISTANCE_SENSOR_SHUTDOWN_PIN 12
#define CONTAINER_DISTANCE_SENSOR_SHUTDOWN_PIN 11

#define HOPPER_DISTANCE_SENSOR_ADDRESS 0x30
#define CONTAINER_DISTANCE_SENSOR_ADDRESS 0x31

Blinker mainAlarm(LED_BUILTIN);
Blinker buzzerAlarm(BUZZER);

unsigned long sequence[6] = {1000,500,300,300,300,300};
unsigned long buzzerSequence[6] = {200,100,200,10000};
unsigned long buzzerDistanceSensorFailedInitialization[2] = {100,100};

enum ProgramStates{
  RUNNING,
  STOPPED
};

ProgramStates programState = ProgramStates::RUNNING;

DistanceSensor hopperDistanceSensor(HOPPER_DISTANCE_SENSOR_SHUTDOWN_PIN);
DistanceSensor containerDistanceSensor(CONTAINER_DISTANCE_SENSOR_SHUTDOWN_PIN);

void setup() {
  Serial.begin(9600);  

  Serial.println("Initializing Hopper Distance sensors");
  containerDistanceSensor.onOff(false);
  hopperDistanceSensor.onOff(false);
  delay(1000);
  hopperDistanceSensor.onOff(true);
  delay(100);
  if(!hopperDistanceSensor.begin()){
    Serial.println("Failed to boot HopperSensor");
    buzzerAlarm.mode(buzzerDistanceSensorFailedInitialization, sizeof(buzzerDistanceSensorFailedInitialization) / sizeof(unsigned long));
    while(1){
      buzzerAlarm.loop();
    }
  }
  hopperDistanceSensor.setAddress(HOPPER_DISTANCE_SENSOR_ADDRESS);
  delay(100);

  Serial.println("Initializing Container Distance Sensor");
  containerDistanceSensor.onOff(true);
  delay(100);
  if(!containerDistanceSensor.begin()){
    Serial.println("Failed to boot DistanceSensor");
    buzzerAlarm.mode(buzzerDistanceSensorFailedInitialization, sizeof(buzzerDistanceSensorFailedInitialization) / sizeof(unsigned long));
    while(1){
      buzzerAlarm.loop();
    }
  }
  containerDistanceSensor.setAddress(CONTAINER_DISTANCE_SENSOR_ADDRESS);
  delay(10);

  Serial.println("Initializing button and potenciometer");
  pinMode(DISTANCE_DIAL, INPUT);
  pinMode(RESET_BUTTON, INPUT);

  Serial.println("Initialization done");
}

void loop(){    
  switch(programState){
    case ProgramStates::RUNNING:{     
      const Measure hopperMeasure = hopperDistanceSensor.getDistance();
      const Measure containerMeasure = containerDistanceSensor.getDistance();
      Serial.println(String(hopperMeasure.range) + ", " + String(containerMeasure.range));

      if(hopperMeasure.range > 30 &&  hopperMeasure.status == VL6180X_ERROR_NONE){
        Serial.println("Stopped, hopper empty");
        programState = ProgramStates::STOPPED;
      }
      if(containerMeasure.range < 150 && containerMeasure.status == VL6180X_ERROR_NONE){
        Serial.println("Stopped, container full");
        programState = ProgramStates::STOPPED;
      }
      break;
    }
    case ProgramStates::STOPPED:{
      if(digitalRead(RESET_BUTTON) == HIGH){        
        Serial.println("Running");
        programState = ProgramStates::RUNNING;
      }
      break;
    }
  }
  buzzerAlarm.loop();
}