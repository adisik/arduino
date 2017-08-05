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
        // Based on: https://github.com/dmaixner/esp8266-nodemcu-aes

        // password used for encypt/decrypt
        byte key[] = {'m', 'y', 's', 'e', 'c', 'r', 'e', 't', 'p', 'a', 's', 's', 'w', 'o', 'r', 'd'};

        AES aes;

        // init vector, for now just constant
        byte iv[N_BLOCK] = {65, 66, 67, 68, 65, 66, 67, 68, 65, 66, 67, 68, 65, 66, 67, 68};

        // plain message in array of bytes
        //TMP byte plain[200];

        // encrypted message
        byte cipher[200];

        // BASE64 encoded data, which are going to be transported to server/storage
        char ivb64[200];
        char cipherb64[200];

        // set password
        aes.set_key(key, sizeof(key));

        // transform string to byte[]
        msg.getBytes(plain, sizeof(plain));
        //printArray("Plain message", plain, msg.length());

        // BASE64 encode init vector
        byte ivb64len = base64_encode(ivb64, (char *)iv, N_BLOCK);
        //printArray("IV", iv, 16);
        //Serial.println("IV in B64: " + String(ivb64));

        // encrypt message with AES128 CBC pkcs7 padding with key and IV
        aes.do_aes_encrypt(plain, strlen((char *)plain), cipher, key, 128, iv);
        //printArray("Encrypted message", cipher, aes.get_size());
        //Serial.println("Encrypted message size: " + String(aes.get_size()));

        // BASE64 encode ciphered message
        byte cipherb64len = base64_encode(cipherb64, (char *)cipher, aes.get_size());
        //Serial.println("Encrypted message in B64: " + String(cipherb64));

        aes.clean();

        // decode IV from BASE64
        base64_decode((char *)iv, ivb64, ivb64len);
        //printArray("IV B64-decoded", iv, 16);

        // decrypt message with AES128 CBC pkcs7 padding with key and IV
        aes.do_aes_decrypt(cipher, aes.get_size(), plain, key, 128, iv);
        //printArray("Decrypted message", plain, msg.length());

      return payload;
    }
};

#endif
