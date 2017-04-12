/*
  Showing number 0-9 on a Common Anode 7-segment LED display
  Displays the numbers 0-9 on the display, with one second inbetween.
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---
    D
  This example code is in the public domain.
 */

#include "DHT.h"

// temperature sensor setup
#define dhtPin 5
#define dhtType DHT11
DHT myDht(dhtPin, dhtType);

// 7-segment display setup
byte pinDot = 27;
byte digit1 = 28;
byte digit2 = 29;
byte digit3 = 30;
byte digit4 = 31;

const byte PIN_NUM = 7;

byte pins[]={32,33,22,23,24,25,26}; // a,b,c,d,e,f,g
byte nums[10][7]={{1,1,1,1,1,1,0},
              {0,1,1,0,0,0,0},
              {1,1,0,1,1,0,1},
              {1,1,1,1,0,0,1},
              {0,1,1,0,0,1,1},
              {1,0,1,1,0,1,1},
              {1,0,1,1,1,1,1},
              {1,1,1,0,0,0,0},
              {1,1,1,1,1,1,1},
              {1,1,1,1,0,1,1}};

const int INTERVAL = 5000;

void setup() {                
  Serial.begin(9600); 
  myDht.begin();

  // initialize the digital pins as outputs.
  for (int i=0;i<PIN_NUM;i++) {
    pinMode(pins[i], OUTPUT);   
  }
  pinMode(pinDot, OUTPUT);   
  pinMode(digit1, OUTPUT);  
  pinMode(digit2, OUTPUT);  
  pinMode(digit3, OUTPUT);  
  pinMode(digit4, OUTPUT);  
}

void ledNumber(byte num[]) {
  for (byte i=0;i<PIN_NUM;i++) {
    digitalWrite(pins[i], num[i]);   
  }
}

byte temperature = 0;
unsigned long previousMillis = 0;

void loop() {
  unsigned long currentMillis = millis();
  if ((temperature==0)||(currentMillis - previousMillis >= INTERVAL)) {
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH); 

    temperature = myDht.readTemperature();
    if (isnan(temperature)) {
      temperature = 0;
    }
    previousMillis = currentMillis;
  }

  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, LOW);
  digitalWrite(digit4, HIGH); 
  ledNumber(nums[temperature/10]);
  delay(10);

  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, LOW); 
  ledNumber(nums[temperature%10]);
  delay(10);
}
