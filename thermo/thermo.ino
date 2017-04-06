#include "Timer.h" // http://playground.arduino.cc/Code/Timer#Installation
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiManager.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 5
// How many bits to use for temperature values: 9, 10, 11 or 12
#define SENSOR_RESOLUTION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature dallas(&oneWire);
DeviceAddress sensorDeviceAddress;

Timer t;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control");

  // Dallas
  dallas.begin();
  dallas.getAddress(sensorDeviceAddress, 0);
  dallas.setResolution(sensorDeviceAddress, SENSOR_RESOLUTION);

  //WiFi manager
  Serial.println("Starting WiFi manager...");
  setupWifi();

  //Timer
  t.every(1000, takeReading);
}

void setupWifi()
{
  WiFiManager wifiManager;
  wifiManager.setTimeout(300);
  wifiManager.autoConnect("ESP8266 Settings");
}



//////////////////////////// LOOP /////////////////////////////

void loop(void)
{ 
  t.update();
}

void takeReading()
{
  //lastExecutionTime = millis();

  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.println("Requesting temperatures...");

  dallas.requestTemperatures();
  float dallas1 = dallas.getTempCByIndex(0);

  if (dallas1 == -127) dallas1 = NAN;

  if (dallas1 == NAN)
  {
    Serial.println("Failed to read from Dallas sensor!");
    return;
  }  

  Serial.println("Dallas | Measured data: " + String(dallas1) + "°C");
}

