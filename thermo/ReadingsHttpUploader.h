#ifndef READINGS_HTTP_UPLOADER_H
#define READINGS_HTTP_UPLOADER_H

#include <ESP8266HTTPClient.h>
#include <AES.h>
#include <ebase64.h>
#include "config.h"
#include "Sensors.h"
#include "DeviceState.h"

class ReadingsHttpUploader
{
    public:

    void processReadings(Sensors &sensors)
    {
        HTTPClient http;
        //String url = "http://blue.pavoucek.cz";
        String url = PUSH_URL;
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
        result += "\"device\": \"";
        result +=  DEVICE_ID;
        result += "\", ";
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
        // Based on: https://github.com/dmaixner/esp8266-nodemcu-aes

        // password used for encypt/decrypt
        byte key[] = PUSH_ENCRYPTION_PASSWORD;

        AES aes;

        // init vector, for now just constant
        byte iv[N_BLOCK] = {65, 66, 67, 68, 65, 66, 67, 68, 65, 66, 67, 68, 65, 66, 67, 68};

        // plain message in array of bytes
        byte plain[PUSH_AES_BUFFER_SIZE];

        // encrypted message
        byte encrypted[PUSH_AES_BUFFER_SIZE];

        // BASE64 encoded data, which are going to be transported to server/storage
        char encrypted64[PUSH_AES_BUFFER_SIZE * 4];

		// BASE64 encoded init vecotr
        char ivb64[N_BLOCK * 4];
			
		// BASE64 encode init vector
    	byte ivb64len = base64_encode(ivb64, (char *)iv, N_BLOCK);
    	printArray("init vector", iv, 16);
    	Serial.println("init vector in base64: " + String(ivb64));

        // set password for encryption
        aes.set_key(key, sizeof(key));

        // transform string to byte[]
        payload.getBytes(plain, sizeof(plain));

        // encrypt message with AES128 CBC pkcs7 padding with key and init vector 
        //aes.do_aes_encrypt(plain, strlen((char *)plain), encrypted, key, 128, iv);
        aes.do_aes_encrypt(plain, payload.length(), encrypted, key, 128, iv);
		printArray("Encrypted message", encrypted, aes.get_size());
        Serial.println("Encrypted message size: " + String(aes.get_size()));

        // BASE64 encode encryted message
        //byte cipherb64len = base64_encode(encrypted64, (char *)encrypted, aes.get_size());

        aes.clean();

        return payload;

        //return String(encrypted64);
    }

	// helper method for printing array of bytes
	void printArray(String name, byte *arr, int length)
	{
		Serial.print(name + ": ");
		for (int i = 0; i < length; i++)
		{
			Serial.write(arr[i]);
		}
		Serial.println();
	}

};

#endif
