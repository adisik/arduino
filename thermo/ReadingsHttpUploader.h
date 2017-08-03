#ifndef READINGS_HTTP_UPLOADER_H
#define READINGS_HTTP_UPLOADER_H

#include <ESP8266HTTPClient.h>
#include "Sensors.h"
#include "DeviceState.h"

class ReadingsHttpUploader
{
    public:
  
    void begin()
    {
        // initialize serial port
        Serial.begin(9600);
        Serial.println("Serial port initialized");
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
                // Serial.println(reading->address + ": " + String(reading->value) + "C");
                reading++;
            }
        }

        HTTPClient http;
        String logPrefix = "[HTTP] ";
        DeviceState::getInstance().debug(logPrefix + "begin...");

        // configure traged server and url
        http.begin("http://blue.pavoucek.cz"); //HTTP

        DeviceState::getInstance().debug(logPrefix + "GET ...");

        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            //USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
            DeviceState::getInstance().debug(logPrefix + "GET ...code:" + httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK)
            {
                String payload = http.getString();
                DeviceState::getInstance().debug(logPrefix + payload);
            }
        } else
        {
            DeviceState::getInstance().debug(logPrefix + "GET... failed, error: " + http.errorToString(httpCode));
        }

        http.end();
    }
};

#endif
