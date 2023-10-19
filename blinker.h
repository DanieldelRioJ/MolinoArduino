#ifndef Blinker_h
#define Blinker_h


#include "Arduino.h"

class Blinker
{
  public:
    Blinker(uint8_t pin, bool activatedWithHigh = true);
    void turnoff();
    void mode(unsigned long *sequence, unsigned long size);
    void loop();

  private:
    uint8_t _pin;
    bool _activatedWithHigh;
    unsigned long _lastTimeChanged;
    unsigned long *_sequence;
    unsigned long _sequenceIndex;
    unsigned long _size;
    bool _activated;
    void _turn(bool onOff);

};

#endif