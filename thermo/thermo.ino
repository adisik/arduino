#include <Wire.h>
#include "Timer.h" // http://playground.arduino.cc/Code/Timer#Installation (https://github.com/JChristensen/Timer)
#include <OneWire.h>
#include "config.h"
#include "WifiConnectivity.h"
#include "ThermoHttpServer.h"
#include "DeviceState.h"
#include "SerialWriter.h"
#include "ReadingsHttpUploader.h"
#include "Lcd.h"
#include "Sensors.h"
#include "SensorDallas.h"

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Instance of global timer
Timer t;

// Instance of device state
//DeviceState deviceState;

// Instance of writer to serial interface
SerialWriter serialWriter;

// Instance of readings uploader (http client)
ReadingsHttpUploader readingsHttpUploader;

// Instance LCD display
Lcd lcd;

// Instance of sensors management
Sensors sensors;

// Instance of Dallas sensor interface
SensorDallas dallas(&oneWire);

// Instance of wifi connectivity wrapper
WifiConnectivity wifiConnectivity;

// Instance of the simple web server
ThermoHttpServer httpServer(&sensors);

void setup(void)
{
  // init device state listeners
  DeviceState::getInstance().addListener(&serialWriter);
  DeviceState::getInstance().addListener(&lcd);
  DeviceState::getInstance().begin();

  // init all sensors
  sensors.addSensor(&dallas);
  sensors.begin();
  
  // init wifi manager
  wifiConnectivity.begin();

  // init http server
  httpServer.begin();

  // configure control loop
  DeviceState::getInstance().state("Control loop");
  t.every(INTERVAL_READINGS, takeReadings);
  t.every(100, handleHttpClient);
}

//////////////////////////// LOOP /////////////////////////////

void loop(void)
{ 
  t.update();
}

void handleHttpClient()
{
  httpServer.handleClient();
}

void takeReadings()
{
  sensors.takeReadings();
  serialWriter.processReadings(sensors);
  readingsHttpUploader.processReadings(sensors);
  lcd.processReadings(sensors);
}


