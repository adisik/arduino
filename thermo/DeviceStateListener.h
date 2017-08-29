#ifndef DEVICEsTATElISTENER_H
#define DEVICEsTATElISTENER_H

#include <Arduino.h>

class DeviceStateListener
{
  public:

  virtual void begin() { };
  virtual void state(String state) { };
  virtual void debug(String msg) { };
};

#endif
