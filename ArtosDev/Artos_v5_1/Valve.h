#ifndef Valve_h
#define Valve_h

#include "Arduino.h"

class Valve
{
  public:
    Valve();
    void setValveTriggerStatusAndTime(int, unsigned long);
    void checkAndCloseValveTriggers(unsigned long); // find all valve triggers that needs to be closed and deactivated
    void forceStop();
    
    int positions[9];
  private:
    
    struct ValveTrigger2
    {
      int positiveTrigger;
      int negativeTrigger;
      int activeTrigger; // -1 the begining positon
      unsigned long triggerStartTime;
    };
    
    ValveTrigger2 _valves2[9];
  
    int findValveTriggerPosition(int);
};

#endif
