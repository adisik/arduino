#ifndef Sensors_h
#define Sensors_h

#include "LinkedList.h"
#include "Sensor.h"

class SensorsListener
{
  public:

  virtual void begin() { };
  virtual void state(String state) { };
  virtual void debug(String msg) { };
};

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
        Sensor *sensor = mySensors.get(i);
        Reading* readings = sensor->takeReadings();
      }
    }
};

#endif
