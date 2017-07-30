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
    
  public:

    SensorDallas(DeviceState *deviceState, OneWire *oneWire)
    {
      oneWire = oneWire;
      
      // Pass our oneWire reference to Dallas Temperature. 
      DallasTemperature dallas(&oneWire);
      DeviceAddress sensorDeviceAddress;
      
    }

  virtual void Initialize()
  {
    Serial.println("Initialize Dallas devices ...");
    dallas.begin();
  
    Serial.print("..found ");
    dallasCount = dallas.getDeviceCount();
    Serial.print(dallasCount, DEC); 
    Serial.println(" devices");

    if (dallasCount > DALLAS_MAX_DEVICES) {
      Serial.print("..too many devices found, truncating to ");
      Serial.println(DALLAS_MAX_DEVICES);
      dallasCount = DALLAS_MAX_DEVICES;
    }
  
    for (int i = 0; i < dallasCount; i++)
    {
      Serial.print("..initialize dallas device ");
      Serial.println(i);
      Serial.print("..");
      dallas.getAddress(dallasAddress[i], i);
      dallas.setResolution(dallasAddress[i], SENSOR_RESOLUTION);
      dallasPrintAddress(dallasAddress[i]);
    }
  }

  void takeReading()
  {
    float dallasTemp[DALLAS_MAX_DEVICES];
    dallasRead(dallasTemp);
    
    Serial.println("Dallas measured data:");
    for (int i = 0; i < dallasCount; i++)
    {
      Serial.print(".. ");
      dallasPrintAddress(dallasAddress[i]);
      Serial.println(": " + String(dallasTemp[i]) + "°C");
    }
    
    // LCD OUTPUT
    //TMP lcd.clear();
    //TMP lcd.home();
      
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
    dallas.requestTemperatures();
    
    for (int i = 0; i < dallasCount; i++)
    {
      temp[i] = dallas.getTempCByIndex(i);
      if (temp[i] == -127) temp[i] = NAN;
    }
  }

  // function to print a dallas device address
  void dallasPrintAddress(DeviceAddress deviceAddress)
  {
    for (uint8_t i = 0; i < 8; i++)
    {
      // zero pad the address if necessary
      if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
    }
  }

};

#endif
