class Sensor
{
  public:
    Sensor(DeviceState *deviceState) = 0;
    virtual void Initialize() = 0;
};
