#ifndef SensorDallas_h
#define SensorDallas_h

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
    OneWire *oneWire = NULL;
    DallasTemperature *dallas = NULL;
    DeviceState *deviceState = NULL;
    
  public:

    SensorDallas(DeviceState *deviceState, OneWire *oneWire)
    {
      this->oneWire = oneWire;
      this->deviceState = deviceState;
    }

  void begin()
  {
    // pass oneWire reference to Dallas Temperature. 
    dallas = new DallasTemperature(oneWire);
    //DeviceAddress sensorDeviceAddress;

    //Serial.print(int(deviceState));
    deviceState->state("Init dallas");

    dallas->begin();
  
    dallasCount = dallas->getDeviceCount();
    String msg = "Found ";
    deviceState->debug(msg + dallasCount + " devices");
    
    if (dallasCount > DALLAS_MAX_DEVICES) {
      msg = "Too many devices found ( ";
      deviceState->debug(msg + dallasCount + ") truncating to " + DALLAS_MAX_DEVICES + " devices");
      dallasCount = DALLAS_MAX_DEVICES;
    }
  
    for (int i = 0; i < dallasCount; i++)
    {
      msg = "Init dallas device";
      deviceState->debug(msg + i);

      // get unique address of sensor
      dallas->getAddress(dallasAddress[i], i);
      
      // set sensor resolution (precision of measurements)
      dallas->setResolution(dallasAddress[i], SENSOR_RESOLUTION);
      
      //TMP dallasPrintAddress(dallasAddress[i]);
      deviceState->debug(dallasAddress2String(dallasAddress[i]));
    }
  }

  Reading* takeReadings()
  {
    float dallasTemp[DALLAS_MAX_DEVICES];
    dallasRead(dallasTemp);
    
    Serial.println("Dallas measured data:");
    for (int i = 0; i < dallasCount; i++)
    {
      Serial.print(".. ");
      //TMP dallasPrintAddress(dallasAddress[i]);
      Serial.println(": " + String(dallasTemp[i]) + "°C");
    }
      
    String lcdInfo;
    lcdInfo = (WiFi.SSID() + " " +  WiFi.localIP().toString() );
    //TMP lcd.print(lcdInfo);
      
    for (int i = 0; i < dallasCount && i < 3; i++)
    {
        //TMP lcd.setCursor(0,i + 1);
        lcdInfo = (String(i) + ": " + String(dallasTemp[i]) + " C");
        //TMP lcd.print(lcdInfo);      
    }
    //TMP lcd.noCursor();
  }

  void dallasRead(float *temp)
  {
    // call sensors.requestTemperatures() to issue a global temperature 
    // request to all devices on the bus
    dallas->requestTemperatures();
    
    for (int i = 0; i < dallasCount; i++)
    {
      temp[i] = dallas->getTempCByIndex(i);
      if (temp[i] == -127) temp[i] = NAN;
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
