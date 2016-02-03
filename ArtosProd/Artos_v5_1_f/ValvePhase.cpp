#include "Arduino.h"
#include "ValvePhase.h"
#include "Valve.h"
#include "ProgramConstants.h"
#include <Time.h>

Valve _valves;

// -----------------//
// Public           // 
// -----------------//
void ValvePhase::switchValvesPosition(int valvePhasePosition)
{
  switch (valvePhasePosition)
  {
    case 1:
    {
      // 1. Zagon
      ValvePhase::switchStartupValvesPosition();
      break;
    }
    case 3:
    {
      // 3. Odzracevanje UF
      ValvePhase::switchVentValvesPosition();
      break;
    }
    case 4:
    {
      // 4. P. spiranje A
      ValvePhase::switchPFlushingAValvesPosition();
      break;
    }
    case 5:
    {
      // 5. P. spiranje B
      ValvePhase::switchPFlushingBValvesPosition();
      break;
    }
    case 6:
    {
      // 6. Delovanje UF + spiranje filtra
      ValvePhase::switchWorkingUFValvesPosition();
      break;
    }
    case 7:
    {
      // 7. Normalno delovanje
      ValvePhase::switchNormalValvesPosition();
      break;
    }
    case 8:
    {
      // 8. Delovanje
      ValvePhase::switchWorkingValvesPosition();
      break;
    }
    case 9:
    {
      // 9. ciscenje Spiranje A
      ValvePhase::switchCleaningFlushingAValvesPosition();
      break;
    }
    case 10:
    {
      // 10. ciscenje P. spiranje A
      ValvePhase::switchCleaningPFlushingAValvesPosition();
      break;
    }
    case 11:
    {
      // 11. ciscenje Spiranje B
      ValvePhase::switchCleaningFlushingBValvesPosition();
      break;
    }
    case 12:
    {
      // 12. ciscenje P. spiranje B
      ValvePhase::switchCleaningPFlushingBValvesPosition();
      break;
    }
    case 13:
    {
      // 13. dezinfekcija
      ValvePhase::switchDesValvesPosition();
      break;
    }
    case 14:
    {
      // 14. koncana dezinfekcija
      ValvePhase::switchEndDesValvesPosition();
      break;
    }
    case 15:
    {
      // 15. zapiranje ventilov
      ValvePhase::switchCloseValvesPosition();
      break;
    }
  }
}

void ValvePhase::checkValveTriggersStatus(unsigned long duration)
{
  _valves.checkAndCloseValveTriggers(duration);
}

void ValvePhase::forceStop()
{
  _valves.forceStop();
}

// -----------------//
// Private          // 
// -----------------//

// 1. Zagon
void ValvePhase::switchStartupValvesPosition()
{
  _pumpRunning = false;
  ValvePhase::switchValves(false, false, false, false, false, true, true, true, false); // 1A
}

// 2. vklopi se crpalka
// 3. Odzracevanje UF
void ValvePhase::switchVentValvesPosition()
{
  _pumpRunning = true;
  ValvePhase::switchValves(true, true, true, true, false, true, true, true, false); // 1B
}

// 4. P. spiranje A
void ValvePhase::switchPFlushingAValvesPosition()
{
  _pumpRunning = true;
  ValvePhase::switchValves(false, true, true, false, false, false, true, true, false); // 1C
}

// 5. P. spiranje B
void ValvePhase::switchPFlushingBValvesPosition()
{
  _pumpRunning = true;
  ValvePhase::switchValves(true, false, false, true, false, false, true, true, false); // 1D
}

// 6. Delovanje UF + spiranje filtra
void ValvePhase::switchWorkingUFValvesPosition()
{
  _pumpRunning = true;
  ValvePhase::switchValves(true, true, false, false, true, false, true, true, false); // 1E
}

// 7. Normalno delovanje
void ValvePhase::switchNormalValvesPosition()
{
  _pumpRunning = true;
  ValvePhase::switchValves(true, true, false, false, true, false, false, false, true); // 2A
}

// 8. Delovanje
void ValvePhase::switchWorkingValvesPosition()
{
  _pumpRunning = true;
  ValvePhase::switchValves(true, true, false, false, true, true, false, false, true); // 2A (intermediate step)
}

// 9. ciscenje Spiranje A
void ValvePhase::switchCleaningFlushingAValvesPosition()
{
  _pumpRunning = true;
  ValvePhase::switchValves(true, false, true, false, false, false, false, false, true); // 2B
}
// 10. ciscenje P. spiranje A
void ValvePhase::switchCleaningPFlushingAValvesPosition()
{
  _pumpRunning = true;
  ValvePhase::switchValves(false, true, true, false, false, false, false, false, true); // 2C
}
// 11. ciscenje Spiranje B
void ValvePhase::switchCleaningFlushingBValvesPosition()
{
  _pumpRunning = true;
  ValvePhase::switchValves(false, true, false, true, false, false, false, false, true); // 2D
}
// 12. ciscenje P. spiranje B
void ValvePhase::switchCleaningPFlushingBValvesPosition()
{
  _pumpRunning = true;
  ValvePhase::switchValves(true, false, false, true, false, false, false, false, true); // 2F
}

// 13. dezinfekcija
void ValvePhase::switchDesValvesPosition()
{
  _pumpRunning = false;
  //ValvePhase::switchValves(false, false, true, true, false, true, false, true, true); // 3
  ValvePhase::switchValves(false, false, true, true, false, true, true, true, true); // 3
}

// 14. koncana dezinfekcija
void ValvePhase::switchEndDesValvesPosition()
{
  _pumpRunning = false;
  ValvePhase::switchValves(false, false, false, false, false, false, false, false, false);
}

// 15. zapiranje ventilov
void ValvePhase::switchCloseValvesPosition()
{
  _pumpRunning = false;
  ValvePhase::switchValves(false, false, false, false, false, false, false, false, false);
}

void ValvePhase::switchValves(boolean valve1, boolean valve2, boolean valve3, boolean valve4, boolean valve5, boolean valve6, boolean valve7, boolean valve8, boolean valve9)
{
  unsigned long time = now();
  if (valve1)
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve1P, time);
  }
  else
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve1M, time);
  }
  
  if (valve2)
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve2P, time);
  }
  else
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve2M, time);
  }
  
  if (valve3)
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve3P, time);
  }
  else
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve3M, time);
  }
  
  if (valve4)
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve4P, time);
  }
  else
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve4M, time);
  }
  
  if (valve5)
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve5P, time);
  }
  else
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve5M, time);
  }
  
  if (valve6)
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve6P, time);
  }
  else
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve6M, time);
  }
  
  if (valve7)
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve7P, time);
  }
  else
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve7M, time);
  }
  
  if (valve8)
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve8P, time);
  }
  else
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve8M, time);
  }
  
  if (valve9)
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve9P, time);
  }
  else
  {
    ValvePhase::activateValveTrigger(ProgramConstants::_valve9M, time);
  }
	
  if (_pumpRunning && valve9)
  {
    digitalWrite(ProgramConstants::_UVLight, HIGH);
  }
  else
  {
    digitalWrite(ProgramConstants::_UVLight, LOW);
  }
}

void ValvePhase::activateValveTrigger(int valveId, unsigned long time)
{
  _valves.setValveTriggerStatusAndTime(valveId, time);
}
