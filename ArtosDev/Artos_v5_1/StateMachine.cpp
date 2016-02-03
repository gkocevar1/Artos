#include "Arduino.h"
#include "StateMachine.h"
#include "ValvePhase.h"
#include "ProgramConstants.h"

#include <Time.h>

unsigned long _runningSequenceStartTime = 0;
unsigned long _interruptionStartTime = 0;
boolean _interruptionRunning = false;

ValvePhase phase;

StateMachine::StateMachine()
{
  StateMachine::initCycles();
}

// -----------------//
// Public           // 
// -----------------//
int StateMachine::runProgramCycle(int cycleId)
{
  // check cycle sequence duration
  phase.checkValveTriggersStatus(-1000);
  
  Cycle cycle = getCycle(cycleId);
  if (cycle.cycleId == -1)
  {
    return -1;
  }
   
  // reset interruption start time
  _interruptionStartTime = 0;
  // set interruption running to init state
  _interruptionRunning = false;
  // go to the first sequence of cycle and execute the sequence
  CycleSequence cycleSequence = cycle.sequence[0];
  // stop valves
  phase.forceStop();
  // set valves position and sequence start time
  StateMachine::updateValvesPositions(cycleSequence.valvePhaseId);
  
  return 0; // started sequence
}

int StateMachine::checkProgramCycleProgress(int cycleId, int sequenceId)
{
  Cycle cycle = getCycle(cycleId);
  CycleSequence cycleSequence = cycle.sequence[sequenceId];
  phase.checkValveTriggersStatus(cycleSequence.duration);
  
  
  
  //if (cycle.cycleId != -1)
  //{
    return StateMachine::checkCycleProgress(cycle, cycleSequence, sequenceId);
  //}
  
  //return -1; // this should never happend
}

boolean StateMachine::isCycleInterruptable(int cycleId)
{
  Cycle cycle = getCycle(cycleId);
  if (cycle.cycleId != -1)
  {
    return cycle.allowCycleInterruption;
  }
  
  return true;
}

int StateMachine::getEndCycleId(int cycleId)
{
  Cycle cycle = getCycle(cycleId);
  if (cycle.cycleId != -1)
  {
    return cycle.endCycleId;
  }
  
  return -1;
}

boolean StateMachine::repeatLastCycle(int cycleId)
{
  Cycle cycle = getCycle(cycleId);
  if (cycle.cycleId != -1)
  {
    return cycle.repeatAfterEnd;
  }
  
  return false;
}

boolean StateMachine::hasInterruption()
{
  return _interruptionRunning;
}

boolean StateMachine::allowNewProgramSelection(int cycleId)
{
  Cycle cycle = getCycle(cycleId);
  if (cycle.cycleId != -1)
  {
    return cycle.allowCycleInterruption;
  }
  
  return false;
}

unsigned long StateMachine::blinkLigth(unsigned long milliSecs, int pin)
{
  unsigned long currentMillis = millis();
  if(currentMillis - milliSecs > 500) 
  {
    milliSecs = currentMillis;
    // if the LED is off turn it on and vice-versa:
    int ledState = digitalRead(pin) == LOW ? HIGH : LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(pin, ledState);
  }
  
  return milliSecs;
}

// -----------------//
// Private          // 
// -----------------//
int StateMachine::checkCycleProgress(Cycle cycle, CycleSequence cycleSequence, int sequenceId)
{
  unsigned long time = now();
  
  if (StateMachine::checkForInprogramInterruption(cycle, cycleSequence, time))
  {
    return sequenceId;
  }
 
  // check if sequence is finished  
  if (_runningSequenceStartTime == 0 && cycle.manuallyTriggered)
  {
    _runningSequenceStartTime = time; // reset running sequence start time; only for manuall next step trigger
    
    // set valves position and sequence start time
    StateMachine::updateValvesPositions(cycleSequence.valvePhaseId);
  }
  
  if ((time - _runningSequenceStartTime) > cycleSequence.duration)
  {
    // update sequence    
    int nextSequenceId = sequenceId + 1;
    if (cycle.manuallyTriggered && (nextSequenceId < cycle.sequencesNo))
    {
      _runningSequenceStartTime = 0; // reset running sequence start time; only for manuall next step trigger
      return _manullyToNextSequence;
    }
    
    if (nextSequenceId > (cycle.sequencesNo - 1)) // init, wash, desinfection
    {
      return _cycleEnd;
    }
    
    cycleSequence = cycle.sequence[nextSequenceId];
    // set valves position and sequence start time
    StateMachine::updateValvesPositions(cycleSequence.valvePhaseId);
    
    return nextSequenceId;
  }
  
  return sequenceId;
}

unsigned int _pumpStatus = -1;
boolean StateMachine::checkForInprogramInterruption(Cycle cycle, CycleSequence cycleSequence, unsigned long time)
{
  if (cycle.hasInProgramInterruption)
  {
    if (_interruptionStartTime == 0)
    {
      // set the interruption start time - when program is started
      _interruptionStartTime = _runningSequenceStartTime;
    }
    
    InterruptCycleSequence ics = cycle.interruptCycleSequence;
    if (_interruptionRunning)
    {
      // switch pump off after 5 seconds and again switch on after 12 seconds
      if ((_pumpStatus == -1) && ((time - _interruptionStartTime) > 5))
      {
        digitalWrite(ProgramConstants::_pumpLight, LOW);
        _pumpStatus = 0;
      }
      else if ((_pumpStatus == 0) && ((time - _interruptionStartTime) > 12))
      {
        digitalWrite(ProgramConstants::_pumpLight, HIGH);
        _pumpStatus = 1;
      }
      
      // check if in-program interruption is completed
      if ((time - _interruptionStartTime) > ics.duration)
      {
        // set normal mode
        phase.switchValvesPosition(cycleSequence.valvePhaseId);
        // set new interruption time
        _interruptionStartTime = time;
        // save information that in program interruption is completed
        _interruptionRunning = false;
        // reset pump status value
        _pumpStatus = -1;
        
        return true; //sequenceId;
      }
    }
    // check for interupption
    else if ((time - _interruptionStartTime) > ics.repeatAfterSeconds)
    {
      // execute interruption cycle
      // set valves position
      phase.switchValvesPosition(ics.valvePhaseId);
      // save information that in-program interruption is running
      _interruptionRunning = true;
      _interruptionStartTime = time;
      
      return true; //sequenceId;
    }
  }
  
  return false;
}

StateMachine::Cycle StateMachine::getCycle(int cycleId)
{
  for (int i = 0; i < sizeof(_cycles); i++)
  {
    Cycle cycle = _cycles[i];
    if (cycle.cycleId == cycleId)
    {
      return cycle;
    }
  }
  
  // this should never happend
  return (Cycle){-1};
}

void StateMachine::updateValvesPositions(int valvePhaseId)
{
  // save sequence start time
  _runningSequenceStartTime = now();
  // set valves position
  phase.switchValvesPosition(valvePhaseId);
}

void StateMachine::initCycles()
{
  // 1. Zagon
  // 2.
  // 3. Odzracevanje UF
  // 4. P. spiranje A
  // 5. P. spiranje B
  // 6. Delovanje UF + Spiranje filtra
  
  // init cycle: 1. (11s) -> 2. (30s) -> 3. (30s) -> 4. (30s) -> 5. (30s) -> 6. (120s)
  _cycles[0] = (Cycle){1, false, true, false, 2, false, 5};
  // next 1.
  _cycles[0].sequence[0] = (CycleSequence){0, 30, 1}; // valve phase id (1. Zagon) // 30
  // next 1. -> 2. (30 seconds)
  _cycles[0].sequence[1] = (CycleSequence){1, 30, 3}; // valve phase id (3. Odzracevanje UF) // 30
  // next 2. -> 3. (30 seconds)
  _cycles[0].sequence[2] = (CycleSequence){2, 30, 4}; // valve phase id (4. P. spiranje A) // 30
  // next 3. -> 4. (30 seconds)
  _cycles[0].sequence[3] = (CycleSequence){3, 30, 5}; // valve phase id (5. P. spiranje B) // 30
  // next 5. -> 6. (120 seconds)
  _cycles[0].sequence[4] = (CycleSequence){4, 120, 6}; // valve phase id (6 . Delovanje UF + Spiranje filtra) //120
  
  // (program 1 duration 900 seconds, program 2 duration 1800 seconds, program 3 duration 3600 seconds)
  // program 1 cycle: 
  // next 7.
  _cycles[1] = (Cycle){2, true, true, true, 5, false, 1};
  _cycles[1].interruptCycleSequence = (InterruptCycleSequence){_prefilterCleaningStart, _prefilterCleaningDuration, 8}; // valve phase id (8. ciscenje predfiltra)
  _cycles[1].sequence[0] = (CycleSequence){0, _program1Duration, 7}; // valve phase id (7. Normalno delovanje)
  
  // program 2 cycle: 
  // next 7.
  _cycles[2] = (Cycle){3, true, true, true, 5, false, 1};
  _cycles[2].interruptCycleSequence = (InterruptCycleSequence){_prefilterCleaningStart, _prefilterCleaningDuration, 8}; // valve phase id (8. ciscenje predfiltra)
  _cycles[2].sequence[0] = (CycleSequence){0, _program2Duration, 7}; // valve phase id (7. Normalno delovanje)
  
  // program 3 cycle: 
  // next 7.
  _cycles[3] = (Cycle){4, true, true, true, 5, false, 1};
  _cycles[3].interruptCycleSequence = (InterruptCycleSequence){_prefilterCleaningStart, _prefilterCleaningDuration, 8}; // valve phase id (8. ciscenje predfiltra)
  _cycles[3].sequence[0] = (CycleSequence){0, _program3Duration, 7}; // valve phase id (7. Normalno delovanje)
  
  // wash cycle only after program 1, 2 or 3:
  // 1. Spiranje A
  // 2. P. spiranje A
  // 3. Spiranje B
  // 4. P. spiranje B 
  // next 1.
  _cycles[4] = (Cycle){5, false, false, false, -1, false, 4};
  _cycles[4].sequence[0] = (CycleSequence){0, _cleaningPhaseDuration, 9}; // valve phase id (9. Spiranje A)
  // next 1. -> 2.
  _cycles[4].sequence[1] = (CycleSequence){1, _cleaningPhaseDuration, 10}; // valve phase id (10. P. spiranje A)
  // next 2. -> 3.
  _cycles[4].sequence[2] = (CycleSequence){2, _cleaningPhaseDuration, 11}; // valve phase id (11. Spiranje B)
  // next 3. -> 4.
  _cycles[4].sequence[3] = (CycleSequence){3, _cleaningPhaseDuration, 12}; // valve phase id (12. P. spiranje B)
  
  // desinfection cycle:
  _cycles[5] = (Cycle){6, false, false, false, -1, true, 2};
  // next 1.
  _cycles[5].sequence[0] = (CycleSequence){0, _desinfectionDuration, 13}; // valve phase id (13. Dezinfekcija) //30
  // next 1. -> 2
  _cycles[5].sequence[1] = (CycleSequence){1, 12, 14}; // valve phase id (14. Koncana dezinfekcija)
}

