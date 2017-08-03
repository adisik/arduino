#ifndef SENSORS_H
#define SENSORS_H

#include "LinkedList.h"
#include "DeviceState.h"
#include "Sensor.h"

class Sensors
{
  private:
    LinkedList<Sensor*> mySensors;
    
  public:
    Sensors()
    {
      mySensors = LinkedList<Sensor*>();
    }
    
    void addSensor(Sensor* sensor)
    {
      mySensors.add(sensor);
    }

    LinkedList<Sensor*> *getSensors() { return &mySensors; }

    void begin()
    {
        DeviceState::getInstance().state("Init sensors");
        
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
