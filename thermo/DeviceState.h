#ifndef DeviceState_h
#define DeviceState_h

#include "LinkedList.h"
#include "DeviceStateListener.h"

class DeviceState
{

  private:
    LinkedList<DeviceStateListener*> myListeners;
    
  public:
    DeviceState()
    {
      myListeners = LinkedList<DeviceStateListener*>();
    }

    void addListener(DeviceStateListener* listener)
    {
      myListeners.add(listener);
    }
};

#endif
