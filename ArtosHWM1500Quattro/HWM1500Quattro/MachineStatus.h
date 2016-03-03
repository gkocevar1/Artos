#ifndef MachineStatus_h
#define MachineStatus_h

#include "Arduino.h"
#include <EEPROM.h>
#include "HWM1500Quattro_debug.h"

class MachineStatus
{
  public:

    /**
       Constructor
    */
    MachineStatus();

    /**
       Check operation time
       Update quarters, and pump operation time
    */
    void checkOperationTime();

    struct Status
    {
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

    // flag indicates that this instance is initialized
    //boolean _initialized = false;
};

#endif
