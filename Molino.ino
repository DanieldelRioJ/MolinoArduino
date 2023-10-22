#include <EEPROM.h>
#include "blinker.h"
#include "distance_sensor.h"
#include "distance_dial.h"
#include "sensor_controller.h"

#define MAX_TIME_WARNING 7000

#define BUZZER 3
#define DISTANCE_DIAL A1
#define RESET_BUTTON 5
#define MAIN_ALARM 7
#define MILL_ELECTRICITY 9

#define HOPPER_DISTANCE_SENSOR_SHUTDOWN_PIN 12
#define CONTAINER_DISTANCE_SENSOR_SHUTDOWN_PIN 11

#define HOPPER_DISTANCE_SENSOR_ADDRESS 0x30
#define CONTAINER_DISTANCE_SENSOR_ADDRESS 0x31

#define STATE_EEPROM_ADDRESS 0

Blinker mainAlarm(LED_BUILTIN);
Blinker buzzerAlarm(BUZZER);
DistanceDial distanceDial(DISTANCE_DIAL);

unsigned long buzzerHopperEmptyWarningSequence[6] = {100,100,100,1000};
unsigned long buzzerContainerFullWarningSequence[6] = {200,1000};
unsigned long buzzerHopperEmptyStoppedSequence[6] = {100,100,100,5000};
unsigned long buzzerContainerFullStoppedSequence[6] = {200,5000};
unsigned long buzzerDistanceSensorFailedInitialization[2] = {100,100};

enum ProgramStates{
  RUNNING,
  WARNING,
  STOPPED
};

ProgramStates programState = ProgramStates::RUNNING;

DistanceSensor hopperDistanceSensor(HOPPER_DISTANCE_SENSOR_SHUTDOWN_PIN);
DistanceSensor containerDistanceSensor(CONTAINER_DISTANCE_SENSOR_SHUTDOWN_PIN);
SensorController sensorController(&hopperDistanceSensor, &containerDistanceSensor);

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
  
  pinMode(MILL_ELECTRICITY, OUTPUT);

  Serial.println("Initialization done");

  recoverAfterTurnOff();
}

void recoverAfterTurnOff(){
  const uint8_t lastState = EEPROM.read(STATE_EEPROM_ADDRESS);
  if(lastState == 1){
    millStoppedContainerFull();
  }else if(lastState == 2){
    millStoppedHopperEmpty();
  }else{
    running();
  }
}

unsigned long firstTimeHopperWarning = 0;
unsigned long firstTimeContainerWarning = 0;

void loop(){    
  const unsigned long actualTime = millis();
  switch(programState){
    case ProgramStates::RUNNING:{     
      checkMeasures();
      break;
    }
    case ProgramStates::WARNING:{
      ControlResults controlResults = checkMeasures();
      if(controlResults.isContainerFull && firstTimeContainerWarning + MAX_TIME_WARNING < actualTime){
        millStoppedContainerFull();
      }else if(controlResults.isHopperEmpty && firstTimeHopperWarning + MAX_TIME_WARNING < actualTime){
        millStoppedHopperEmpty();
      }
      break;
    }
    case ProgramStates::STOPPED:{
      digitalWrite(MILL_ELECTRICITY, HIGH);
      if(digitalRead(RESET_BUTTON) == HIGH){     
        running();
      }
      break;
    }
  }
  buzzerAlarm.loop();
}

ControlResults checkMeasures(){
  const float potenciometerRead = distanceDial.getDistance();
  sensorController.setContainerThreshold(potenciometerRead);
  ControlResults controlResults = sensorController.getControlResults();
  Serial.println(controlResults.isContainerFull);
  if(controlResults.isContainerFull){
    if(programState == ProgramStates::RUNNING){
      firstTimeContainerWarning = millis();
      containerFull();
    }    
  }else if(controlResults.isHopperEmpty){     
    if(programState == ProgramStates::RUNNING){
      firstTimeHopperWarning = millis();
      hopperEmpty();
    }
  }else{
    running();
  }
  return controlResults;
}

void running(){    
  digitalWrite(MILL_ELECTRICITY, LOW);
  if(programState == ProgramStates::STOPPED){    
    sensorController.reset();
  }
  EEPROM.update(STATE_EEPROM_ADDRESS, 0);   
  programState = ProgramStates::RUNNING;
  buzzerAlarm.turnoff();
}

void containerFull(){
  Serial.println("WARNING ContainerFull");
  programState = ProgramStates::WARNING;
  buzzerAlarm.mode(buzzerContainerFullWarningSequence, sizeof(buzzerContainerFullWarningSequence) / sizeof(unsigned long));
}

void hopperEmpty(){
  Serial.println("WARNING HopperEmpty");
  programState = ProgramStates::WARNING;
  buzzerAlarm.mode(buzzerHopperEmptyWarningSequence, sizeof(buzzerHopperEmptyWarningSequence) / sizeof(unsigned long));
}


void millStoppedContainerFull(){
  Serial.println("STOPPED ContainerFull");
  EEPROM.update(STATE_EEPROM_ADDRESS, 1);  
  programState = ProgramStates::STOPPED;
  buzzerAlarm.mode(buzzerContainerFullStoppedSequence, sizeof(buzzerContainerFullStoppedSequence) / sizeof(unsigned long));
}

void millStoppedHopperEmpty(){  
  Serial.println("STOPPED HopperEmpty");
  EEPROM.update(STATE_EEPROM_ADDRESS, 2);  
  programState = ProgramStates::STOPPED;
  buzzerAlarm.mode(buzzerHopperEmptyStoppedSequence, sizeof(buzzerHopperEmptyStoppedSequence) / sizeof(unsigned long));
}