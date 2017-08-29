#ifndef DEVICESTATE_H
#define DEVICESTATE_H

#include "LinkedList.h"
#include "DeviceStateListener.h"

// Singleton that provides methods for reporting both
// device state and debug messages
class DeviceState
{
  private:
    LinkedList<DeviceStateListener*> myListeners;

    DeviceState()
    {
      myListeners = LinkedList<DeviceStateListener*>();
    }

  public:

    static DeviceState& getInstance()
    {
        static DeviceState instance; // Guaranteed to be destroyed.
                                     // Instantiated on first use.
        return instance;
    }

    // technique of deleting the methods we don't want.
    DeviceState(DeviceState const&) = delete;
    void operator=(DeviceState const&)  = delete;
  
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
