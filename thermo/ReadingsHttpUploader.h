#ifndef READINGS_HTTP_UPLOADER_H
#define READINGS_HTTP_UPLOADER_H

#include <ESP8266HTTPClient.h>
#include "Sensors.h"
#include "DeviceState.h"

class ReadingsHttpUploader
{
    public:

    void processReadings(Sensors &sensors)
    {
        HTTPClient http;
        //String url = "http://blue.pavoucek.cz";
        String url = "http://192.168.0.12:9200/api/readings";
        String logPrefix = "[HTTP] ";

        // configure traged server and url
        http.begin(url); //HTTP

        http.addHeader("Content-Type", "application/json");

        DeviceState::getInstance().debug(logPrefix + "POST to " + url);

        int httpCode = http.POST(encodePayload(readings2Json(sensors)));

        // httpCode will be negative on error
        if(httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            DeviceState::getInstance().debug(logPrefix + "POST status code: " + httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK)
            {
                String payload = http.getString();
                DeviceState::getInstance().debug(logPrefix + payload);
            }
        } else
        {
            DeviceState::getInstance().debug(logPrefix + "POST failed, error: " + http.errorToString(httpCode));
        }

        http.end();
    }

    private:

    String readings2Json(Sensors &sensors)
    {
        String result = "";
        result += "{";
        result += "\"device\": \"thermo\", ";
        result += "\"readings\": [";
        
        // get all sensor readings and generate appropriate HTML representation
        for(int i = 0; i < sensors.getSensors()->size(); i++)
        {
            Sensor *sensor = sensors.getSensors()->get(i);
            Reading* reading = sensor->getReadings();
            bool readingCounter = 0;
            while (reading->value != Reading::VALUE_LAST)
            {
                // Serial.println(reading->address + ": " + String(reading->value) + "C");
                if (readingCounter > 0) result += ",";
                result += "{\"address\": \"" + reading->address + "\", \"value\": " + reading->value + "}";
                reading++;
                readingCounter++;
            }
        }
        
        result += "]}";
        
        return result;
    }

    String encodePayload(String payload)
    {
      // TODO
      return payload;
    }
};

#endif
