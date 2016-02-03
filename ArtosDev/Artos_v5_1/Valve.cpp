#include "Arduino.h"
#include "Valve.h"
#include "ProgramConstants.h"
#include <Time.h>

Valve::Valve()
{
  _valves2[0] = (ValveTrigger2){ ProgramConstants::_valve1P, ProgramConstants::_valve1M, -1, 0 }; // -1 the begining position
  _valves2[1] = (ValveTrigger2){ ProgramConstants::_valve2P, ProgramConstants::_valve2M, -1, 0 };
  _valves2[2] = (ValveTrigger2){ ProgramConstants::_valve3P, ProgramConstants::_valve3M, -1, 0 };
  _valves2[3] = (ValveTrigger2){ ProgramConstants::_valve4P, ProgramConstants::_valve4M, -1, 0 };
  _valves2[4] = (ValveTrigger2){ ProgramConstants::_valve5P, ProgramConstants::_valve5M, -1, 0 };
  _valves2[5] = (ValveTrigger2){ ProgramConstants::_valve6P, ProgramConstants::_valve6M, -1, 0 };
  _valves2[6] = (ValveTrigger2){ ProgramConstants::_valve7P, ProgramConstants::_valve7M, -1, 0 };
  _valves2[7] = (ValveTrigger2){ ProgramConstants::_valve8P, ProgramConstants::_valve8M, -1, 0 };
  _valves2[8] = (ValveTrigger2){ ProgramConstants::_valve9P, ProgramConstants::_valve9M, -1, 0 };
}

// -----------------//
// Public           // 
// -----------------//
void Valve::setValveTriggerStatusAndTime(int valveId, unsigned long time)
{
  // find valve trigger
  int pos = findValveTriggerPosition(valveId);
  if (pos == -1)
  {
    return;
  }
  
  if (_valves2[pos].activeTrigger == valveId) // current valve is active
  {
    return;
  } 
  else if (_valves2[pos].activeTrigger != -1)
  {
    // stop partner trigger id
    digitalWrite(_valves2[pos].activeTrigger, LOW);
    delay(400); // this happend only when program is running (switching position of valve 6)
    
    _valves2[pos].activeTrigger = valveId;
    digitalWrite(valveId, HIGH);
    return;
  }
  
  // save active valve
  _valves2[pos].activeTrigger = valveId;
  _valves2[pos].triggerStartTime = time;
  // open valve trigger
  digitalWrite(valveId, HIGH);
}

void Valve::checkAndCloseValveTriggers(unsigned long duration)
{
  unsigned long time = now();
  for (int i = 0; i < 9; i++)
  {
    if (((_valves2[i].activeTrigger != -1) && (time - _valves2[i].triggerStartTime) > (duration - 2))) // close valve trigger 2 seconds before cycle sequence ends
    {
      // close valve trigger
      digitalWrite(_valves2[i].activeTrigger, LOW);
      _valves2[i].activeTrigger = -1;
    }
  }
}

void Valve::forceStop()
{
  for (int i = 0; i < 9; i++)
  {
    if (_valves2[i].activeTrigger != -1)
    {
      // close valve trigger
      digitalWrite(_valves2[i].activeTrigger, LOW);
      _valves2[i].activeTrigger = -1;
    }
  }
  
  delay(400);
}

// -----------------//
// Private          // 
// -----------------//
int Valve::findValveTriggerPosition(int valveId)
{
  for (int i = 0; i < 9; i++)
  {
    ValveTrigger2 vt = _valves2[i];
    if (vt.positiveTrigger == valveId || vt.negativeTrigger == valveId)
    {
      return i;
    }
  }
  
  return -1; // this should never happend
}





