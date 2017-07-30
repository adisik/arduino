#include <LiquidCrystal.h>

#include <Wire.h>

#include <LiquidCrystal_I2C.h> // hacked version of https://github.com/agnunez/ESP8266-I2C-LCD1602 (begin uses default arguments passed to Wire.begin())

#include <DNSServer.h>

#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiScan.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <WiFiServer.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiType.h>

//#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Timer.h" // http://playground.arduino.cc/Code/Timer#Installation
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiManager.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 0
// How many bits to use for temperature values: 9, 10, 11 or 12
#define SENSOR_RESOLUTION 12

#define DALLAS_MAX_DEVICES 5

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
int dallasCount              = 0; // number of available dallas sensors
DeviceAddress dallasAddress[DALLAS_MAX_DEVICES];

// Construct an LCD object and pass it the 
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup(void)
{
  // initialize serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control");

  // initialize display
  lcd.begin();
  //lcd.init();
  lcd.backlight();
  //lcd.noBacklight();
  lcd.clear(); lcd.home();
  
  // Dallas
  lcd.clear(); lcd.home();
  lcd.print("Init Dallas");
  setupDallas();
  
  //Hostname
  host = host_prefix + device_id;
  host.toLowerCase();
  Serial.println("Device hostname is " + host);

  // WiFi manager
  Serial.println("Initialize WiFi manager...");
  lcd.clear(); lcd.home();
  lcd.print("Init WiFi");
  setupWifi();

  // Http Server
  lcd.clear(); lcd.home();
  lcd.print("Init WebServer");
  Serial.println("Initialize WebServer...");
  setupWebServer();

  //Timer
  t.every(5000, takeReading);
  t.every(100, handleHttpClient);
}

void setupDallas()
{
  Serial.println("Initialize Dallas devices ...");
  dallas.begin();
  
  Serial.print("..found ");
  dallasCount = dallas.getDeviceCount();
  Serial.print(dallasCount, DEC); 
  Serial.println(" devices");

  if (dallasCount > DALLAS_MAX_DEVICES) {
    Serial.print("..too many devices found, truncating to ");
    Serial.println(DALLAS_MAX_DEVICES);
    dallasCount = DALLAS_MAX_DEVICES;
  }

  for (int i = 0; i < dallasCount; i++)
  {
    Serial.print("..initialize dallas device ");
    Serial.println(i);
    Serial.print("..");
    dallas.getAddress(dallasAddress[i], i);
    dallas.setResolution(dallasAddress[i], SENSOR_RESOLUTION);
    dallasPrintAddress(dallasAddress[i]);
  }
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

void takeReading()
{
  float dallasTemp[DALLAS_MAX_DEVICES];
  dallasRead(dallasTemp);
  
  Serial.println("Dallas measured data:");
  for (int i = 0; i < dallasCount; i++)
  {
    Serial.print(".. ");
    dallasPrintAddress(dallasAddress[i]);
    Serial.println(": " + String(dallasTemp[i]) + "°C");
  }
  
  // LCD OUTPUT
  lcd.clear();
  lcd.home();
    
  String lcdInfo;
  lcdInfo = (WiFi.SSID() + " " +  WiFi.localIP().toString() );
  lcd.print(lcdInfo);
    
  for (int i = 0; i < dallasCount && i < 3; i++)
  {
      lcd.setCursor(0,i + 1);
      lcdInfo = (String(i) + ": " + String(dallasTemp[i]) + " C");
      lcd.print(lcdInfo);      
  }
  lcd.noCursor();
}

void handleHttpClient() {
  httpServer.handleClient();
}

void dallasRead(float *temp)
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  dallas.requestTemperatures();
  
  for (int i = 0; i < dallasCount; i++)
  {
    temp[i] = dallas.getTempCByIndex(i);
    if (temp[i] == -127) temp[i] = NAN;
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

    float dallasTemp[DALLAS_MAX_DEVICES];
    dallasRead(dallasTemp);
  
    for (int i = 0; i < dallasCount; i++)
    {
      info += ("<b>DS18B20 Readings for device " + String(i) + ":</b> " + String(dallasTemp[i]) + "&#8451;<br />");
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

// function to print a dallas device address
void dallasPrintAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

