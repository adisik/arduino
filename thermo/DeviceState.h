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

    void begin()
    {
      DeviceStateListener *listener;
    
      for(int i = 0; i < myListeners.size(); i++)
      {
        listener = myListeners.get(i);
        listener->begin();
      }
    }

    void state(String state)
    {
      DeviceStateListener *listener;
    
      for(int i = 0; i < myListeners.size(); i++)
      {
        listener = myListeners.get(i);
        listener->state(state);
      }
    }

    void debug(String msg)
    {
      DeviceStateListener *listener;
    
      for(int i = 0; i < myListeners.size(); i++)
      {
        listener = myListeners.get(i);
        listener->debug(msg);
      }
    }
};

#endif
