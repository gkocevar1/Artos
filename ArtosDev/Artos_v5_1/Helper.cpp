#include "Arduino.h"
#include "Helper.h"
#include "EEPROMHelper.h"
#include "ProgramConstants.h"
#include <EEPROM.h>
#include <Time.h>

/*
To reset service timer press up - select - up - select within 5 seconds
*/
// reset button combination
int _resetCombination[4] = {1, 4, 1, 4}; 
// pressed combination
int _combination[4] = {0, 0, 0, 0};
// quarters start time
unsigned long _quarterStartTime;
// pressed combination start time
unsigned long _combinationStartTime;

EEPROMHelper eepromHelper;

Helper::Helper()
{
  _quarterStartTime = -1; 
  _combinationStartTime = 0;
}

// -----------------//
// Public           // 
// -----------------//
int Helper::getOperationTime()
{
  int operation = eepromHelper.ReadInt(0);
  return operation;
}

int Helper::getFromServiceTime()
{
  int fromService = eepromHelper.ReadInt(20);
  return fromService;
}

int Helper::getQuarters()
{
  int quarters = eepromHelper.ReadInt(30);
  return quarters;
}

void Helper::checkOperationTime()
{
   // return;
  
  unsigned long time = now();
  if (_quarterStartTime == -1 || (time - _quarterStartTime > (15*60)))
  {
    _quarterStartTime = time;
    //Helper::updateOperationTime();
  }
}

void Helper::updateOperationTime()
{
   // return;
    
  int quarters = getQuarters();
  if (++quarters > 4)
  {
    int operationHours = getOperationTime();
    eepromHelper.WriteInt(0, ++operationHours);
    eepromHelper.WriteSingleValue(30, 1); // reset quarters
  }
  else
  {
    eepromHelper.WriteSingleValue(30, quarters);
  }
  
}

unsigned long time1  = now();
int Helper::needService()
{
  int operation = getOperationTime();
  int fromService = getFromServiceTime();
    /*
  if (now() - time1 == 100)
  {
    return 290;
  }
  else if ((now() - time1) == 130)
  {
    return 300;
  }
  return (int)now();*/
        
  return operation - fromService;
}

boolean Helper::resetServiceCounter(int value)
{
  
  
  // 1 - up
  // 4 - select
  // valid combination: 1, 4, 1, 4 within 5 seconds
  unsigned long time = now();
  if (value == 1)
  {
    if (_combinationStartTime == 0)
    {
      _combinationStartTime = time;
      _combination[0] = 1;
    }
    else if ((time - _combinationStartTime) < 5)
    {
      _combination[2] = 1;
    }
    else
    {
      resetInputCombination(1);
    }
  }
  else if (value == 4)
  {
    if (_combinationStartTime > 0 && ((time - _combinationStartTime) < 5))
    {
      if (_combination[1] == 4)
      {
        _combination[3] = 4;
 
        // check all
        boolean reset = true;
        for (int i = 0; i < 4; i++)
        {
          if (_resetCombination[i] != _combination[i])
          {
            reset = false;
          }
        }
  
        if (reset)
        {
          int operationHours = getOperationTime();
          
          eepromHelper.WriteInt(30, 1); // reset quarters
          delay(500);
          eepromHelper.WriteInt(20, operationHours); // set service time
          delay(500);
          resetInputCombination(4);
          
          // counter is reset
          return true;
        }
      }
      else
      {
        _combination[1] = 4;
      }
    }
    else if (_combinationStartTime > 0)
    {
      resetInputCombination(4);
    }
  }
  
  Serial.print(_combination[0]);
    Serial.print(" ");
  Serial.print(_combination[1]);
      Serial.print(" ");
  Serial.print(_combination[2]);
      Serial.print(" ");
  Serial.print(_combination[3]);
  Serial.println("");
  
  return false;
}

// -----------------//
// Private          // 
// -----------------//
void Helper::resetInputCombination(int value)
{
  _combinationStartTime = 0;
  for (int i = 0; i < 4; i++)
  {
    _combination[i] = 0;
    if (i == 0 && value == 1)
    {
      _combination[i] = 1;
    }
  }
}
