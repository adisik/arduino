//#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
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

ESP8266WebServer httpServer(80);

const String device_id       = "Thermo1";       // Device indentification
const String host_prefix     = "ESP8266";      // Hostname prefix
String host;

void setup(void)
{
  // initialize serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control");

  // Dallas
  Serial.println("Initialize Dallas sensors ...");
  dallas.begin();
  dallas.getAddress(sensorDeviceAddress, 0);
  dallas.setResolution(sensorDeviceAddress, SENSOR_RESOLUTION);

  //Hostname
  host = host_prefix + device_id;
  host.toLowerCase();
  Serial.println("Device hostname is " + host);

  // WiFi manager
  Serial.println("Initialize WiFi manager...");
  setupWifi();

  // Http Server
  Serial.println("Initialize WebServer...");
  setupWebServer();

  //Timer
  t.every(1000, takeReading);
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

void takeReading() {

  float dallas1;
  readDallas(dallas1);
  
  Serial.println("Dallas | Measured data: " + String(dallas1) + "°C");
}

void handleHttpClient() {
  httpServer.handleClient();
}
void readDallas(float &dallas1)
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.println("Requesting temperatures...");

  dallas.requestTemperatures();
  dallas1 = dallas.getTempCByIndex(0);

  if (dallas1 == -127) dallas1 = NAN;

  if (dallas1 == NAN)
  {
    Serial.println("Failed to read from Dallas sensor!");
    return;
  }  
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
    float dallas1;
    readDallas(dallas1);
    
    info += ("<b>DS18B20 Readings:</b> " + String(dallas1) + "&#8451;<br />");

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
