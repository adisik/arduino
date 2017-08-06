#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h> // hacked version of https://github.com/agnunez/ESP8266-I2C-LCD1602 (begin uses default arguments passed to Wire.begin())
#include <LiquidCrystal.h>
#include "config.h"
#include "DeviceStateListener.h"

class Lcd: public DeviceStateListener
{
    private:
        LiquidCrystal_I2C *lcd;
    
    public:

    Lcd()
    {
      // Construct an LCD object and pass it the 
      // I2C address, width (in characters) and
      // height (in characters). Depending on the
      // Actual device, the IC2 address may change.
      lcd = new LiquidCrystal_I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);
    }
  
    void begin()
    {
      // initialize display
      lcd->begin();
      //lcd->begin(0,2);      // In ESP8266-01, SDA=0, SCL=2 
      //lcd.init();

      // turn on display backlight 
      lcd->backlight();

      // clear content
      lcd->clear();

      // move cursor to home position
      lcd->home();
    }

    void state(String state)
    {
      lcd->clear();
      lcd->home();
      lcd->print(state);
    }

    void processReadings(Sensors &sensors)
    {
        lcd->clear();
        lcd->home();

        // get all sensor readings and generate appropriate HTML representation
        for(int i = 0; i < sensors.getSensors()->size(); i++)
        {
            Sensor *sensor = sensors.getSensors()->get(i);
            Reading* reading = sensor->getReadings();
            bool readingCounter = 0;
            while (reading->value != Reading::VALUE_LAST)
            {
 				lcd->setCursor(0, readingCounter);
				lcd->print(String(i) + "-" + String(readingCounter) + ": " + String(reading->value) + " C");

                reading++;
                readingCounter++;
            }
        }
        
		lcd->noCursor();
    }

};

#endif
