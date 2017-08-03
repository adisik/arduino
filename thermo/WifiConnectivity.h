#ifndef WIFICONNECTIVITY_H
#define WIFICONNECTIVITY_H

#include <Ethernet.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiScan.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <WiFiServer.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiType.h>
#include <WiFiManager.h>
#include "DeviceState.h"

class WifiConnectivity
{
  public:
  
    void begin()
    {
        DeviceState::getInstance().state("Init wifi manager");
        //wifiManager.resetSettings();  // this can be used to reset rembembered SID
        WiFiManager wifiManager;
        wifiManager.setTimeout(300);
        wifiManager.autoConnect("ESP8266 Settings");
    }
};    

#endif
