#ifndef SENSORDALLAS_H
#define SENSORDALLAS_H

#include <DallasTemperature.h>
#include "Sensor.h"

// How many bits to use for temperature values: 9, 10, 11 or 12
#define SENSOR_RESOLUTION 12

// Max number of devices
#define DALLAS_MAX_DEVICES 5

class SensorDallas : public Sensor
{
  private:
    int dallasCount = 0; // number of available dallas sensors
    DeviceAddress dallasAddress[DALLAS_MAX_DEVICES];
    Reading readings[DALLAS_MAX_DEVICES + 1];
    OneWire *oneWire = NULL;
    DallasTemperature *dallas = NULL;
    
  public:

    SensorDallas(OneWire *oneWire)
    {
      this->oneWire = oneWire;
    }

  void begin()
  {
    // pass oneWire reference to Dallas Temperature. 
    dallas = new DallasTemperature(oneWire);
    //DeviceAddress sensorDeviceAddress;

    //Serial.print(int(deviceState));
    DeviceState::getInstance().state("Init dallas");

    dallas->begin();
  
    dallasCount = dallas->getDeviceCount();
    String msg = "Found ";
    DeviceState::getInstance().debug(msg + dallasCount + " devices");
    
    if (dallasCount > DALLAS_MAX_DEVICES) {
      msg = "Too many devices found ( ";
      DeviceState::getInstance().debug(msg + dallasCount + ") truncating to " + DALLAS_MAX_DEVICES + " devices");
      dallasCount = DALLAS_MAX_DEVICES;
    }
  
    for (int i = 0; i < dallasCount; i++)
    {
      msg = "Init dallas device";
      DeviceState::getInstance().debug(msg + i);

      // get unique address of sensor
      dallas->getAddress(dallasAddress[i], i);
      
      // set sensor resolution (precision of measurements)
      dallas->setResolution(dallasAddress[i], SENSOR_RESOLUTION);
      
      //TMP dallasPrintAddress(dallasAddress[i]);

      readings[i].address = dallasAddress2String(dallasAddress[i]);
      DeviceState::getInstance().debug(dallasAddress2String(dallasAddress[i]));
    }

    readings[dallasCount + 1].value = Reading::VALUE_LAST;
  }

  Reading* takeReadings()
  {
    float dallasTemp[DALLAS_MAX_DEVICES];
    dallasRead(dallasTemp);
  
    for (int i = 0; i < dallasCount && i < 3; i++)
    {
      readings[i].value = dallasTemp[i];
    }
    readings[dallasCount].value = Reading::VALUE_LAST;

    return getReadings();
  }

  Reading* getReadings()
  {
    return readings;
  }

  void dallasRead(float *temp)
  {
    // call sensors.requestTemperatures() to issue a global temperature 
    // request to all devices on the bus
    dallas->requestTemperatures();
    
    for (int i = 0; i < dallasCount; i++)
    {
      temp[i] = dallas->getTempCByIndex(i);
      if (temp[i] == -127) temp[i] = Reading::VALUE_NA;
    }
  }

  // get string representation of a dallas device address
  String dallasAddress2String(DeviceAddress deviceAddress)
  {
    String result = "";
    for (uint8_t i = 0; i < 8; i++)
    {
      // zero pad the address if necessary
      if (deviceAddress[i] < 16) result += "0";
      
      result += String(deviceAddress[i], HEX);
    }
    return result;
  }
};

#endif
