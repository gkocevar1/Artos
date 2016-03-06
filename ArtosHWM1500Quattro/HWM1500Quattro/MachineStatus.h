#ifndef MachineStatus_h
#define MachineStatus_h

#include "Arduino.h"
#include <EEPROM.h>
#include <Time.h>
#include <Vector2.h>
#include "HWM1500Quattro_debug.h"
#include "Constants.h"

class MachineStatus
{
  public:

    /**
       Constructor
    */
    MachineStatus();

    /**
       Check operation time
       Update quarters, and pump operation time at specified period

       return true if service is needed - no cycles will be able to execute
    */
    boolean checkOperationTime();

    /**
     * reset operation time (counter)
     * 
     * to reset operation time following keys must be presed within 5 seconds: 1, 4, 1, 4 (up, select, up, select)
     * 
     * 
     * @param1: key
     */
    boolean resetOperationTime(int);

    /**
       Blink light on specified pin (state is changed every 500 millisecond)

       @param1: last check time
       @param2: pin number
    */
    unsigned long blinkLigth(unsigned long, int);

    struct Status
    {
       // special configuration version
      char version[4];
      
      // pump total operation time (in hours)
      unsigned int operationTime;

      // last service time
      unsigned int serviceTime;

      // quaters (15 min)
      // every 15 minutes quarter is updated.
      // after four periods (1 hour), hour is to operation time and quarters are reset to 1
      // 1 = 0-15min
      // 2 = 15-30min
      // 3 = 30-45min
      // 4 = 45-60min
      short int quarters;
    };

    Status machineStatus;

  private:

    /**
       read machine status from eeprom
    */
    void init();

    /**
       updates pump operation time
    */
    void updateOperationTime();

    // quarter time
    long _quarterTime = -1;
    // last blink check time (for service light - power on light)
    unsigned long _lastBlinkCheckTime = -1;

    // service reset function match combination
    static int _matchCombination[];
    // service timer (combination must be pressed with 5 seconds)
    long _resetCounterTime = -1;
    // user press combination
    Vector<int> _userPressCombination;
};

#endif
