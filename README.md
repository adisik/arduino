# arduino

## Installation of IDE

Don't install Aruion IDE from official Ubuntu repositories, the version is too old
and doesn't allow easy configuration of new boards. The better is to download tar aechive
from official IDE pages and follow installation instructions (http://ubuntuhandbook.org/index.php/2015/11/install-arduino-ide-1-6-6-ubuntu/)

### Registration of ESP board

Add following to Preferences -> Additional Boards Manager URLs: `http://arduino.esp8266.com/versions/2.3.0/package_esp8266com_index.json` and
install ESP8266 boards via Tools -> Board -> Boards manager

## I2C display wiring:
`SCL->D1`
`SDA->D2`
`VCC->VV`
`GND->GND`

Gotchas: set contrast on the LCD display (solution: connect power to 5V pin instead of 3V pin)
