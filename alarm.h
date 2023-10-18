#ifndef Alarm_h
#define Alarm_h

#define TIME_SHORT_RINGING_ON 500
#define TIME_SHORT_RINGING_OFF 200 

#define TIME_LONG_RINGING_ON 1000 
#define TIME_LONG_RINGING_OFF 1000 

#define TIME_STILL_ACTIVE_RINGING_ON 200 
#define TIME_STILL_ACTIVE_RINGING_OFF 10000 


#include "Arduino.h"

enum AlarmMode{
  deactivated = 0,
  short_ringing = 1,
  long_ringing = 2,
  still_active_ringing = 3
};

class Alarm
{
  public:
    Alarm(uint8_t pin);
    void mode(AlarmMode mode);
    void loop();

  private:
    uint8_t _pin;
    unsigned long _lastTimeChangedToOn;
    unsigned long _lastTimeChangedToOff;
    bool _ringing;
    AlarmMode _mode;

    void _ring(unsigned long timeRingingOn, unsigned long timeRingingOff);
    void _turn(bool onOff);

};

#endif