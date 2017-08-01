
#include <Wire.h>

#include <DNSServer.h>

#include <LiquidCrystal_I2C.h> // hacked version of https://github.com/agnunez/ESP8266-I2C-LCD1602 (begin uses default arguments passed to Wire.begin())
#include <LiquidCrystal.h>

#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiScan.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <WiFiServer.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Timer.h" // http://playground.arduino.cc/Code/Timer#Installation
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiManager.h>
#include "DeviceState.h";
#include "SerialWriter.h";
#include "Lcd.h"
#include "Sensors.h"
#include "SensorDallas.h"

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 0

// Prefix of device network host name
#define HOST_NAME "thermo1"

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Instance of global timer
Timer t;

// Instance of device state
DeviceState deviceState;

// Instance of writer to serial interface
SerialWriter serialWriter;

// Instance LCD display
Lcd lcd;

// Instance of sensors management
Sensors sensors;

// Instance of Dallas sensor interface
SensorDallas dallas(&deviceState, &oneWire);

// Instance of the simple web server
ESP8266WebServer httpServer(80);

String host = HOST_NAME;

void setup(void)
{
  deviceState.addListener(&serialWriter);
  deviceState.addListener(&lcd);
  deviceState.begin();
  
  deviceState.state("Init sensors");

  sensors.addSensor(&dallas);
  sensors.begin();
  
  // create dallas sensor instance
 
  // Dallas
  //TMP lcd.clear(); lcd.home();
  //TMP lcd.print("Init Dallas");
  //TMP setupDallas();
  
  //Hostname
  //TMP host = HOST_PREFIX + DEVICE_ID;
  host.toLowerCase();
  deviceState.debug("Device hostname is " + host);

  // WiFi manager
  deviceState.state("Init WiFi manager");
  //TMP lcd.clear(); lcd.home();
  //TMP lcd.print("Init WiFi");
  setupWifi();

  // Http Server
  //TMP lcd.clear(); lcd.home();
  //TMP lcd.print("Init WebServer");
  deviceState.state("Init WebServer");
  setupWebServer();

  deviceState.state("Control loop");
  //Timer
  //TMP t.every(5000, takeReading);
  t.every(100, handleHttpClient);
}

void setupWifi()
{
  //wifiManager.resetSettings();  // this can be used to reset rembembered SID
  WiFiManager wifiManager;
  wifiManager.setTimeout(300);
  wifiManager.autoConnect("ESP8266 Settings");
}

void setupWebServer()
{
  char login[20], passwd[20];
  
  MDNS.begin(host.c_str());
  //httpUpdater.setup(&httpServer, update_path, update_username.c_str(), update_password.c_str());
  httpServer.on("/", HTTP_GET, []() {
    httpServer.sendHeader("Connection", "close");
    httpServer.sendHeader("Access-Control-Allow-Origin", "*");
    httpServer.send(200, "text/html", deviceStatus());
  });
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
}


//////////////////////////// LOOP /////////////////////////////

void loop(void)
{ 
  t.update();
}

void handleHttpClient() {
  httpServer.handleClient();
}

String deviceStatus()
{
    String info;

    info  = "<!DOCTYPE HTML><html><head><title>ESP8266 device</title><meta charset='utf-8' />";
    info += "<style>html{font-family: sans-serif}</style></head>";
    info += "<body><h1>ESP8266 device</h1>";

    info += ("<b>Hostname:</b> ");
    info += (host + ".local [<a href='//" + host + ".local'>open</a>]");
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

    Serial.println();

    //TMP float dallasTemp[DALLAS_MAX_DEVICES];
    //TMP dallasRead(dallasTemp);
  
    //TMP for (int i = 0; i < dallasCount; i++)
    //TMP {
    //TMP   info += ("<b>DS18B20 Readings for device " + String(i) + ":</b> " + String(dallasTemp[i]) + "&#8451;<br />");
    //TMP }

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


