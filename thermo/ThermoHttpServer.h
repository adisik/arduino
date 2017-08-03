#ifndef THERMOHTTPSERVER_H
#define THERMOHTTPSERVER_H

#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
#include "DeviceState.h"
#include "Sensors.h"

class ThermoHttpServer : public RequestHandler
{
  private:
    ESP8266WebServer *httpServer = NULL;
    Sensors *sensors = NULL;
    String hostName;
    
  public:
    ThermoHttpServer(Sensors* sensors): RequestHandler()
    {
      this->hostName = "thermo";
      this->sensors = sensors;
      httpServer = new ESP8266WebServer(80);
    }

    void begin()
    {
      char login[20];
      char passwd[20];

      DeviceState::getInstance().state("Init WebServer");
      DeviceState::getInstance().debug("Device hostname is " + this->hostName);
      
      MDNS.begin(this->hostName.c_str());
      httpServer->addHandler(this);
      httpServer->begin();
      MDNS.addService("http", "tcp", 80);
    }

    bool canHandle(HTTPMethod method, String uri)
    {
      return (method == HTTP_GET);
    }

    bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri)
    {
      if (!canHandle(requestMethod, requestUri))
        return false;
        
      // send content
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/html", deviceStatus());
      
      return true;
    }

    void handleClient()
    {
      httpServer->handleClient();
    }
    
    String deviceStatus()
    {
        String info;
    
        info  = "<!DOCTYPE HTML><html><head><title>ESP8266 device</title><meta charset='utf-8' />";
        info += "<style>html{font-family: sans-serif}</style></head>";
        info += "<body><h1>ESP8266 device</h1>";
    
        info += ("<b>Hostname:</b> ");
        info += (hostName + ".local [<a href='//" + hostName + ".local'>open</a>]");
        info += ("<br />");
    
        info += ("<b>IP address:</b> ");
        info += (WiFi.localIP().toString());
        info += ("<br />");
    
        byte mac[6];
    
        info += ("<b>MAC address:</b> ");
        info += mac2String(WiFi.macAddress(mac));
        info += ("<br />");
    
        info += ("<b>WiFi network (SSID):</b> ");
        info += (WiFi.SSID());
        info += ("<br />");
    
        info += ("<b>Signal strength (RSSI):</b> ");
        info += (WiFi.RSSI());
        info += (" dBm");
        info += ("<br />");

        // get all sensor readings and generate appropriate HTML representation
        for(int i = 0; i < this->sensors->getSensors()->size(); i++)
        {
          Sensor *sensor = this->sensors->getSensors()->get(i);
          Reading* reading = sensor->getReadings();
          while (reading->value != Reading::VALUE_LAST)
          {
            info += ("<b>" + reading->address + ":</b> " + String(reading->value) + "&#8451;<br />");
            reading++;
          }
        }
    
        info += ("</body></html>");
       
        return info;
    }

    String mac2String(byte ar[])
    {
      String s;
      for (byte i = 0; i < 6; ++i)
      {
        char buf[3];
        sprintf(buf, "%2X", ar[i]);
        s += buf;
        if (i < 5) s += ':';
      }
      return s;
    }
    
};

#endif
