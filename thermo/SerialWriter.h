#include "DeviceStateListener.h"

class SerialWriter: public DeviceStateListener
{
  public:
  
    SerialWriter()
    {
        // initialize serial port
        Serial.begin(9600);
        Serial.println("Dallas Temperature IC Control");
    }
  
    void Initialize()
    {

    }
};
