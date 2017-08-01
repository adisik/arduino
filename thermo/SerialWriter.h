#ifndef SerialWriter_h
#define SerialWriter_h

#include "DeviceStateListener.h"

class SerialWriter: public DeviceStateListener
{
  public:
  
    SerialWriter()
    {

    }
  
    void begin()
    {
        // initialize serial port
        Serial.begin(9600);
        Serial.println("Serial port initialized");
    }

    void state(String state)
    {
        Serial.println("New state: " + state);
    }

    void debug(String msg)
    {
        Serial.println(msg);
    }
};

#endif
