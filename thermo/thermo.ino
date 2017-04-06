#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 5
// How many bits to use for temperature values: 9, 10, 11 or 12
#define SENSOR_RESOLUTION 12
// Index of sensors connected to data pin, default: 0
#define SENSOR_INDEX 0

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
DeviceAddress sensorDeviceAddress;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
  sensors.getAddress(sensorDeviceAddress, 0);
  sensors.setResolution(sensorDeviceAddress, SENSOR_RESOLUTION);
}

void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.println("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures

  float temperatureInCelsius = sensors.getTempCByIndex(SENSOR_INDEX);
  float temperatureInFahrenheit = sensors.getTempFByIndex(SENSOR_INDEX);

  Serial.print("C Temperature for the device 1 is: ");
  Serial.println(temperatureInCelsius, 4);
  Serial.print("F Temperature for the device 1 is: ");
  Serial.println(temperatureInFahrenheit, 4);
  
  //Serial.println(sensors.getTempCByIndex(SENSOR_INDEX));  

  delay(10000);
}
