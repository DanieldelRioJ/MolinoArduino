#include "Blinker.h"
#include "Arduino.h"

//HIGH IS RELÉ WITH CIRCUIT CLOSED
//DOWN IS RELÉ WITH CIRCUIT OPENED

Blinker::Blinker(uint8_t pin, bool activatedWithHigh){
  this->_pin = pin;  
  pinMode(this->_pin, OUTPUT);
  this->turnoff();
  this->_activatedWithHigh = activatedWithHigh;
  this->_lastTimeChanged = 0;
};

void Blinker::mode(unsigned long *sequence, unsigned long size){
  this->_sequenceIndex = 0;
  this->_size = size;
  this->_activated = true;
  this->_sequence = sequence;  
};

void Blinker::loop(){  
  if(this->_activated){
    unsigned long actualTime = millis();
    if(this->_lastTimeChanged + this->_sequence[this->_sequenceIndex] < actualTime){
      this->_lastTimeChanged = actualTime;
      this->_sequenceIndex = (this->_sequenceIndex + 1) % this->_size;
      this->_turn(this->_sequenceIndex % 2 == 0);
    }
  }else{
    this->_turn(false);
  }
}

void Blinker::turnoff(){
  this->_activated = false;
  this->_turn(false);
}

void Blinker::_turn(bool onOff){
  if(this->_activatedWithHigh){
    digitalWrite(this->_pin, onOff ? HIGH : LOW);
  }else{    
    digitalWrite(this->_pin, onOff ? LOW : HIGH);
  }
}