#ifndef Blinker_h
#define Blinker_h


#include "Arduino.h"

class Blinker
{
  public:
    Blinker(uint8_t pin, bool activatedWithHigh = true);
    void turnoff();
    void mode(unsigned long *sequence, uint8_t size);
    void loop();

  private:
    uint8_t _pin;
    bool _activatedWithHigh;
    unsigned long _lastTimeChanged;
    unsigned long *_sequence;
    uint8_t _sequenceIndex;
    uint8_t _size;
    bool _activated;
    void _turn(bool onOff);

};

#endif