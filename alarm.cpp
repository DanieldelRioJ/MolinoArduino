#include "alarm.h"
#include "Arduino.h"

//HIGH IS RELÉ WITH CIRCUIT CLOSED
//DOWN IS RELÉ WITH CIRCUIT OPENED

Alarm::Alarm(uint8_t pin){
  this->_pin = pin;  
  pinMode(this->_pin, OUTPUT);
  this->_turn(false);
  this->_mode = AlarmMode::deactivated;
  this->_lastTimeChangedToOn = 0;
  this->_lastTimeChangedToOff = 0;
};

void Alarm::mode(AlarmMode mode){
  this->_mode = mode;
};

void Alarm::loop(){
  switch(this->_mode){
    case AlarmMode::short_ringing:{
      this->_ring(TIME_SHORT_RINGING_ON, TIME_SHORT_RINGING_OFF);
      break;
    }
    case AlarmMode::long_ringing:{
      this->_ring(TIME_LONG_RINGING_ON, TIME_LONG_RINGING_OFF);
      break;
    }
    case AlarmMode::still_active_ringing:{
      this->_ring(TIME_STILL_ACTIVE_RINGING_ON, TIME_STILL_ACTIVE_RINGING_OFF);
      break;
    }

    default:{
      this->_turn(false);
    }
  }
}

void Alarm::_ring(unsigned long timeRingingOn, unsigned long timeRingingOff){
  unsigned long actualTime = millis();
  if(this->_ringing && actualTime > timeRingingOn + this->_lastTimeChangedToOn ){
    this->_lastTimeChangedToOff = actualTime;
    this->_turn(false);
  }else if(!this->_ringing && actualTime > timeRingingOff + this->_lastTimeChangedToOff){
    this->_lastTimeChangedToOn = actualTime;
    this->_turn(true);
  }
}

void Alarm::_turn(bool onOff){
  this->_ringing = onOff;
  digitalWrite(this->_pin, !onOff ? LOW: HIGH);
}