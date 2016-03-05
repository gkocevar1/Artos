#include "MachineStatus.h"

/**
   Constructor
*/
MachineStatus::MachineStatus()
{
  MachineStatus::init();
}

// ------------------
// Public methods

/**
   Check operation time
   Update quarters, and pump operation time at specified period

   return true if service is needed - no cycles will be able to execute
*/
boolean MachineStatus::checkOperationTime()
{
  // reset resetCounterTime if user didn't match reset combination, also clear all user presses
  if (_resetCounterTime != -1 && (now() - _resetCounterTime) > 5)
  {
    _resetCounterTime = -1;
    _userPressCombination.clear();
  }
  
  // find time from last service
  long timeFromService = machineStatus.operationTime - machineStatus.serviceTime;
  if (timeFromService >= 390)
  {
    // service will be needed soon, just blink with light
    _lastBlinkCheckTime = MachineStatus::blinkLigth(_lastBlinkCheckTime, Constants::PowerOnLight);
  }
  else if (timeFromService >= 400)
  {
    // service is required immediatelly, do not start with wash cycle
    _lastBlinkCheckTime = MachineStatus::blinkLigth(_lastBlinkCheckTime, Constants::PowerOnLight);
    return true;
  }

  unsigned long current = now();
  // update quarter immediately after pump is turned on
  if (_quarterTime == -1 || (current - _quarterTime > (15 * 60)))
  {
    _quarterTime = current;

    if (++machineStatus.quarters > 4)
    {
      // update operation hours
      machineStatus.operationTime++;
      // reset quarters
      machineStatus.quarters = 1;
    }

    DMSG1("Update operation time to: "); DMSG(machineStatus.operationTime);
    DMSG1("Update quarters to: "); DMSG(machineStatus.quarters);

    //MachineStatus::updateOperationTime();
  }

  return false;
}

/**
   reset operation time (counter)

   to reset operation time following keys must be presed within 5 seconds: 1, 4, 1, 4 (up, select (enter), up, select (enter))

   @param1: key
*/

int MachineStatus::_matchCombination[] = {1, 4, 1, 4};
boolean MachineStatus::resetOperationTime(int key)
{
  if (_resetCounterTime == -1)
  {
    // set reset counter
    _resetCounterTime = now();
  }

  _userPressCombination.push_back(key);
  if (_userPressCombination.size() == 4)
  {
    for(int i = 0; i < 4; i++)
    {
      // if one of input combination doesn't match, clear everything
      if (MachineStatus::_matchCombination[i] != _userPressCombination[i])
      {
        _userPressCombination.clear();
        return false;
      }
    }

    DMSG("RESET Service Counter");
    
    machineStatus.serviceTime = machineStatus.operationTime;
    machineStatus.operationTime = 1;
    
    //MachineStatus::updateOperationTime();
    _resetCounterTime = -1;
    _userPressCombination.clear();

    return true;
  }

  return false;
}

/**
   Blink light on specified pin (state is changed every 500 millisecond)

   @param1: last check time
   @param2: pin number
*/
unsigned long MachineStatus::blinkLigth(unsigned long milliSecs, int pin)
{
  unsigned long currentMillis = millis();
  if (currentMillis - milliSecs > 500)
  {
    milliSecs = currentMillis;
    // if the LED is off turn it on and vice-versa:
    int ledState = digitalRead(pin) == LOW ? HIGH : LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(pin, ledState);
  }

  return milliSecs;
}

// ------------------
// Private methods

/**
   read machine status from eeprom
*/
void MachineStatus::init()
{
  for (unsigned int t = 0; t < sizeof(machineStatus); t++) {
    *((char*)&machineStatus + t) = EEPROM.read(t);
  }

  //machineStatus.operationTime = 391;
}

/**
   updates pump operation time
*/
void MachineStatus::updateOperationTime()
{
  for (unsigned int t = 0; t < sizeof(machineStatus); t++) {
    EEPROM.write(t, *((char*)&machineStatus + t));
  }
}
