#ifndef DeviceStateListener_h
#define DeviceStateListener_h

#include <Arduino.h>

class DeviceStateListener
{
  public:

  virtual void begin() { };
  virtual void state(String state) { };
  virtual void debug(String msg) { };
};

#endif
