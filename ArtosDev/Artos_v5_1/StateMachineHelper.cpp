#include "Arduino.h"
#include "StateMachineHelper.h"
#include "StateMachine.h"
#include "ValvePhase.h"
#include "ProgramConstants.h"
#include <Time.h>

// currently runnning cycle
int _runningCycleId = -1; // init state is -1
// currently runnning sequence of the cycle
int _runningCycleSequenceId = -1; // init state is -1
// repeatable cycle id
int _repeatCycleId = -1; // init state is -1 (we don't need to execute repeatable program)
// is used to save last running program (program 1,2,3) (only to display running program on display)
int _lastRunningProgram = -1;
// flag (if true) indicates that we need to stop everything
boolean _stopEverything = false;
// manually selected program (program 1, 2, 3, desinfection)
int _programState = 0;
// manually selected wash
boolean _wash = false;

unsigned long _desinfectionLight;
char* _firstLine;
char* _secondLine;

StateMachine state;

StateMachineHelper::StateMachineHelper()
{
  _allowNewProgramSelection = false;
  _waitForManuallTrigger = false;
}


// -----------------//
// Public           // 
// -----------------//
StateMachineHelper::StateMachineResult StateMachineHelper::runCycle(int cycleId)
{
  runProgram(cycleId);
  return setResult();
}

StateMachineHelper::StateMachineResult StateMachineHelper::processCycle()
{
  checkCycleStatus();
  return setResult();
}

StateMachineHelper::StateMachineResult StateMachineHelper::stopCycle()
{
  stopPrivate();
  return setResult();
}

void StateMachineHelper::setProgramState(int programState)
{
  _programState = programState;
}

void StateMachineHelper::activateWash()
{
  _wash = true;
}

int StateMachineHelper::getProgramState()
{
  return _programState;
}

void StateMachineHelper::switchAllProgramLights(int ledState)
{
  digitalWrite(ProgramConstants::_program1Light, ledState);
  digitalWrite(ProgramConstants::_program2Light, ledState);
  digitalWrite(ProgramConstants::_program3Light, ledState);
  digitalWrite(ProgramConstants::_desinfectionLight, ledState);
}

void StateMachineHelper::setRunningCycleSequenceId(int runningCycleSequenceId)
{
  _runningCycleSequenceId = runningCycleSequenceId;
}

unsigned long StateMachineHelper::blinkLigth(unsigned long milliSecs, int pin)
{
  return state.blinkLigth(milliSecs, pin);
}

// -----------------//
// Private          // 
// -----------------//
void StateMachineHelper::runProgram(int cycleId)
{
  _runningCycleId = cycleId;
  // run program
  _runningCycleSequenceId = state.runProgramCycle(_runningCycleId);
  // check if new program can be selected
  _allowNewProgramSelection = state.allowNewProgramSelection(_runningCycleId);
  // display running program
  displayRunningProgram();
  // check running cycle status
  checkCycleStatus(); 
}

void StateMachineHelper::displayRunningProgram()
{
  switch (_runningCycleId)
  {
    case 1:
    //case 7:
    {
      updateStartUpSequence(0, true);
      break;
    }
    case 2:
    {
      _lastRunningProgram = 1;
      updateProgramSequence(StateMachine::_displayRunningCycle);
      break;
    }
    case 3:
    {
      _lastRunningProgram = 2;
      updateProgramSequence(StateMachine::_displayRunningCycle);
      break;
    }
    case 4:
    {
      _lastRunningProgram = 3;
      updateProgramSequence(StateMachine::_displayRunningCycle);
      break;
    }
    case 5:
    {
      updateWashSequence(0, true);
      break;
    }
    case 6:
    {
      updateDesinfectionSequence(0, true);
      break;
    }
  }
}

void StateMachineHelper::checkCycleStatus()
{
  int returnedSequenceId = state.checkProgramCycleProgress(_runningCycleId, _runningCycleSequenceId);
  if (returnedSequenceId == StateMachine::_manullyToNextSequence)
  {
    _waitForManuallTrigger = true;
  }
  
  boolean update = returnedSequenceId != _runningCycleSequenceId;
  if (returnedSequenceId != _runningCycleSequenceId)
  {
    // update current running cycle sequence
    _runningCycleSequenceId = returnedSequenceId;
  }
  
  switch (_runningCycleId)
  {
    case 1: // start-up and stand alone wash cycle
    //case 7: // stand alone wash cycle
    {
      updateStartUpSequence(returnedSequenceId, update);
      break;
    }
    case 2: // program 1 cycle
    case 3: // program 2 cycle
    case 4: // program 3 cycle
    {
      updateProgramSequence(returnedSequenceId);
      break;
    }
    case 5:
    {
      // after program 1, 2 or 3
      updateWashSequence(returnedSequenceId, update);
      break;
    }
    case 6:
    {
      update = update ? update : returnedSequenceId == 1;
      updateDesinfectionSequence(returnedSequenceId, update);
      break;
    }
  }
}

void StateMachineHelper::updateStartUpSequence(int sequenceId, boolean update)
{
  if (!update)
  {
    return;
  }
  
  switch (sequenceId)
  {
    case 0: // start up
    {
      if (!_wash)
      {
        printToFirstLine("Initializing");
      }
      else
      {
        printToFirstLine("Washing");
        _wash = false;
      }
      
      printToSecondLine("Startup");
      
      switchAllProgramLights(LOW);
      digitalWrite(ProgramConstants::_flushingLight, HIGH);
      
      if (_programState == 2)
      {
        digitalWrite(ProgramConstants::_program2Light, HIGH);
      }
      else if (_programState == 3)
      {
        digitalWrite(ProgramConstants::_program3Light, HIGH);
      }
      else
      {
        digitalWrite(ProgramConstants::_program1Light, HIGH);
      }
      
      break;
    }
    case 1:
    {
      printToSecondLine("venting UF");
      break;
    }
    case 2:
    {
      printToSecondLine("flushing A");
      break;
    }
    case 3:
    {
      printToSecondLine("flushing B");
      break;
    }
    case 4:
    {
      printToSecondLine("UF + flushing");
      break;
    }
    case StateMachine::_cycleEnd:
    {
      processProgramEnd();
      
      break;
    }
    default:
    {
      printToSecondLine("Error: Init");
      digitalWrite(ProgramConstants::_errorLight, HIGH);
      delay(20000);
      break;
    }
  }
}

boolean _lastState = true;
void StateMachineHelper::updateProgramSequence(int sequenceId)
{
  switch (sequenceId)
  {
    case StateMachine::_displayRunningCycle:
    {
      printToSecondLine("running");
      switchAllProgramLights(LOW);
      digitalWrite(ProgramConstants::_flushingLight, LOW);
      
      if (_lastRunningProgram == 1)
      {
        printToFirstLine("Program: 1");
        digitalWrite(ProgramConstants::_program1Light, HIGH);
      }
      else if (_lastRunningProgram == 2)
      {
        printToFirstLine("Program: 2");
        digitalWrite(ProgramConstants::_program2Light, HIGH);
      }
      else if (_lastRunningProgram = 3)
      {
        printToFirstLine("Program: 3");
        digitalWrite(ProgramConstants::_program3Light, HIGH);
      }
      break;  
    }
    case 0: // program is running
    {
      boolean interrupt = state.hasInterruption();
      // to prevent instant refresh
      if (_lastState == interrupt)
      {
        return;
      }
      else
      {
        _lastState = interrupt;
      }
      
      if (interrupt)
      {
        printToSecondLine("pre-cleaning");
      }
      else
      {
        printToSecondLine("running");
      }
      break;
    }
    case StateMachine::_cycleEnd:
    {
      switchAllProgramLights(LOW);
      processProgramEnd();
      break;
    }
    default:
    {
      printToSecondLine("Error: Init");
      digitalWrite(ProgramConstants::_errorLight, HIGH);
      delay(20000);
      break;
    }
  }
}

void StateMachineHelper::updateWashSequence(int sequenceId, boolean update)
{
  if (!update)
  {
    return;
  }
  
  switch (sequenceId)
  {
    case 0:
    {
      switchAllProgramLights(LOW);
      digitalWrite(ProgramConstants::_flushingLight, HIGH);
      if (_lastRunningProgram == 1)
      {
        printToFirstLine("Program: 1 wash");
        digitalWrite(ProgramConstants::_program1Light, HIGH);
      }
      else if (_lastRunningProgram == 2)
      {
        printToFirstLine("Program: 2 wash");
        digitalWrite(ProgramConstants::_program2Light, HIGH);
      }
      else if (_lastRunningProgram == 3)
      {
        printToFirstLine("Program: 3 wash");
        digitalWrite(ProgramConstants::_program3Light, HIGH);
      }
    
      printToSecondLine("clean pos. 1");
      break;
    }
    case 1: // program is running
    {
      printToSecondLine("clean pos. 2");
      break;
    }
    case 2: // program is running
    {
      printToSecondLine("clean pos. 3");
      break;
    }
    case 3: // program is running
    {
      printToSecondLine("clean pos. 4");
      break;
    }
    case StateMachine::_cycleEnd:
    {
      digitalWrite(ProgramConstants::_flushingLight, LOW);
      
      processProgramEnd();
      break;
    }
    default:
    {
      printToSecondLine("Error: Wash");
      digitalWrite(ProgramConstants::_errorLight, HIGH);
      delay(20000);
      break;
    }
  }
}

void StateMachineHelper::updateDesinfectionSequence(int sequenceId, boolean update)
{
  if (!update)
  {
    return;
  }
  
  switch (sequenceId)
  {
    case 0: // desinfection
    {
      digitalWrite(ProgramConstants::_flushingLight, LOW);
      digitalWrite(ProgramConstants::_desinfectionLight, HIGH);
      printToFirstLine("Program: desinf.");
      printToSecondLine("running");
      break;
    }
    case 1: // ending desinfection - manually select end
    {
      _desinfectionLight = state.blinkLigth(_desinfectionLight, ProgramConstants::_desinfectionLight);
      break;
    }
    case StateMachine::_manullyToNextSequence:
    {
      printToSecondLine("press enter");
      // overrwrite this function to allow manually move to next sequence
      _allowNewProgramSelection = true;
      _waitForManuallTrigger = true;
      
      break;
    }
    case StateMachine::_cycleEnd:
    {
      _stopEverything = true; // machine is prepared to be switched off when desinfection process ends
      //printToFirstLine("Desinf. end");
      printToSecondLine("completed");      
      break;
    }
    default:
    {
      printToSecondLine("Error: Desinf.");
      digitalWrite(ProgramConstants::_errorLight, HIGH);
      delay(20000);
      break;
    }
  }
}

void StateMachineHelper::processProgramEnd()
{
  // check if cycle is repeatable or has defined end cycle - end cycle has higher priority
  // run new cycle after end 
  // save state to repeat last program
  if (state.repeatLastCycle(_runningCycleId))
  {
    _repeatCycleId = _runningCycleId;
  }
  
  if (_runningCycleId == 1 && (_programState > 0 && _programState < 4))
  {
    _runningCycleId = _programState + 1;
  }
  else
  {
    // save running cycle id
    _runningCycleId = state.getEndCycleId(_runningCycleId);
  }
  
  if (_runningCycleId == -1)
  {
    _runningCycleId = _repeatCycleId;
  }
  
  if (_runningCycleId == -1)
  {
    stopPrivate(); // no programs specified after end; stop everything
  }
  else
  {
    runProgram(_runningCycleId);
  }
}

void StateMachineHelper::stopPrivate()
{
  // stop all programs
  _runningCycleId = -1;
  _runningCycleSequenceId = -1;
  _repeatCycleId = -1;
  _lastRunningProgram = -1;
  _allowNewProgramSelection = true;
  _waitForManuallTrigger = false;
  switchAllProgramLights(LOW);
  printToFirstLine("");
  printToSecondLine("");
}

void StateMachineHelper::printToFirstLine(char *text)
{
  _firstLine = text;
}

void StateMachineHelper::printToSecondLine(char *text)
{
  _secondLine = text;
}

StateMachineHelper::StateMachineResult StateMachineHelper::setResult()
{
  StateMachineResult result = { _firstLine, _secondLine, _stopEverything };
  return result;
}
