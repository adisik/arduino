#ifndef Sensor_h
#define Sensor_h

class Reading
{
  public:
    static const int valueNA = -1;
    static const int valueLast = -9999;
    
    String address;
    float value;

    Reading() : address(""), value(valueNA) {}
};

class Sensor
{
  public:
    //Sensor(DeviceState *deviceState);
    virtual void begin() = 0;

    // force sensor(s) to take readings
    virtual Reading* takeReadings();
};

#endif
