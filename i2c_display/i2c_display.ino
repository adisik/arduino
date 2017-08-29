#include <Wire.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  //lcd.init();   // initializing the LCD
  lcd.begin();   // initializing the LCD
  //lcd.backlight(); // Enable or Turn On the backlight 
  lcd.setCursor(0, 0);
  lcd.print("1234567890123456"); // Start Print text to Line 1
  lcd.setCursor(0, 1);      
  lcd.print("abcdefghijklmnop"); // Start Print Test to Line 2
}

void loop() {
}
