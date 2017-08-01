#include "DeviceStateListener.h"

#include <LiquidCrystal_I2C.h> // hacked version of https://github.com/agnunez/ESP8266-I2C-LCD1602 (begin uses default arguments passed to Wire.begin())
#include <LiquidCrystal.h>

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
      lcd = new LiquidCrystal_I2C(0x27, 20, 4);
    }
  
    void begin()
    {
      // initialize display
      lcd->begin();
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
};
