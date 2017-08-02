#ifndef Sensors_h
#define Sensors_h

#include "LinkedList.h"
#include "DeviceState.h"
#include "Sensor.h"

class Sensors
{
  private:
    LinkedList<Sensor*> mySensors;
    DeviceState *deviceState = NULL;
    
  public:
    Sensors(DeviceState *deviceState)
    {
      this->deviceState = deviceState;
      mySensors = LinkedList<Sensor*>();
    }
    
    void addSensor(Sensor* sensor)
    {
      mySensors.add(sensor);
    }

    LinkedList<Sensor*> *getSensors() { return &mySensors; }

    void begin()
    {
      deviceState->state("Init sensors");
        
      for(int i = 0; i < mySensors.size(); i++)
      {
        Sensor *sensor = mySensors.get(i);
        sensor->begin();
      }
    }

    void takeReadings()
    {
      for(int i = 0; i < mySensors.size(); i++)
      {
        mySensors.get(i)->takeReadings();
      }
    }
};

#endif
