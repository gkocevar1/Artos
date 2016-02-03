#ifndef Helper_h
#define Helper_h

#include "Arduino.h"

#include <Time.h>

/** 
*	
*	Update working hours and reset working hours
*       Working with EEPROM
*       We use 6 addresses from EEPROM:
*       0, 1: are used for operation hours
*       30: is used to mark 15 minutes interval (quarters)
*       20, 21: are used to save hours from last service
**/
// update every 15 minutes

class Helper
{
  public:
    Helper();
    int getOperationTime();
    int getFromServiceTime();
    int getQuarters();
    void checkOperationTime();
    int needService();
    boolean resetServiceCounter(int);
    void updateOperationTime();
    
  private:
    void resetInputCombination(int);
};

#endif
