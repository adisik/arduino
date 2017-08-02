#ifndef SerialWriter_h
#define SerialWriter_h

#include "DeviceStateListener.h"
#include "Sensors.h"

class SerialWriter: public DeviceStateListener
{
  public:
  
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

    void processReadings(Sensors &sensors)
    {
      // get all sensor readings and generate appropriate HTML representation
      for(int i = 0; i < sensors.getSensors()->size(); i++)
      {
        Sensor *sensor = sensors.getSensors()->get(i);
        Reading* reading = sensor->getReadings();
        while (reading->value != Reading::VALUE_LAST)
        {
          Serial.println(reading->address + ": " + String(reading->value) + "C");
          reading++;
        }
      }
    }
};

#endif
