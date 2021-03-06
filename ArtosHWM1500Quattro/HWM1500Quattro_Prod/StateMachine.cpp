#include "StateMachine.h"

/**
   Constructor
*/
StateMachine::StateMachine()
{
  StateMachine::init();
}

// ------------------
// Public methods

/**
   run program

   @param1: program to run
   @param2: flag indicates if method is called when machine is turned on. In that case wash program is selected by default and program 1 must be selected as second cycle.
*/
void StateMachine::runProgram(Constants::Program program, boolean start)
{
  if (!start && program == StateMachine::runningProgram)
  {
    return;
  }

  // new program is selected manually
  if (!start)
  {
    // deactivate all valves
    _vp.deactivateValves(500);
  }

  switch (program)
  {
    case Constants::Program::Program1:
      {
        // set sequences for program 1
        digitalWrite(Constants::Program1Light, HIGH);
        StateMachine::setFiltrationSequences(Constants::Program::Program1);
        StateMachine::start(StateMachine::_cycles[1]);

        break;
      }
    case Constants::Program::Program2:
      {
        // set sequences for program 2
        digitalWrite(Constants::Program2Light, HIGH);
        StateMachine::setFiltrationSequences(Constants::Program::Program2);
        StateMachine::start(StateMachine::_cycles[1]);

        break;
      }
    case Constants::Program::Program3:
      {
        // set sequences for program 3
        digitalWrite(Constants::Program3Light, HIGH);
        StateMachine::setFiltrationSequences(Constants::Program::Program3);
        StateMachine::start(StateMachine::_cycles[1]);

        break;
      }
    case Constants::Program::ProgramWash:
      {
        if (start)
        {
          // program 1 is run automatically when pump is turned on
          StateMachine::programToRunAfterWash = Constants::Program::Program1;
          // set sequences for program 1
          StateMachine::setFiltrationSequences(Constants::Program::Program1);
          digitalWrite(Constants::Program1Light, HIGH);
        }
        else
        {
          // indicates that program is interrupted with wash cycle
          // wash is triggered manually (save states, because running program must continue at the exact same position as program was interrupted)
          // save sequence
          StateMachine::_programSequence = StateMachine::_sequenceNumber;
          // save running time
          StateMachine::_programSequenceDuration = now() - StateMachine::_sequenceStart;
          // save program
          StateMachine::programToRunAfterWash = StateMachine::runningProgram;
        }

        digitalWrite(Constants::WashLight, HIGH);
        StateMachine::start(StateMachine::_cycles[0]);

        break;
      }
    case Constants::Program::ProgramDesinfection:
      {
        digitalWrite(Constants::DesinfectionLight, HIGH);
        StateMachine::start(StateMachine::_cycles[3]);

        break;
      }
    case Constants::Program::ProgramClose:
      {
        digitalWrite(Constants::DesinfectionLight, HIGH);
        StateMachine::start(StateMachine::_cycles[4]);

        break;
      }
  }

  // set current running program
  StateMachine::runningProgram = program;
}

/**
   Check current cycle progress

   skip progress check, if current sequence duration is equals to -1 (unlimited duration)
*/
void StateMachine::checkProgress()
{
  if (StateMachine::runningProgram == Constants::Program::ProgramNone)
  {
    return;
  }

  int duration = StateMachine::getSequenceDuration();
  if (duration > -1)
  {
    if (now() > (duration + StateMachine::_sequenceStart))
    {
      // move to next sequence
      StateMachine::moveToNextSequence();
    }
    else if ((now() + 2) > (duration + StateMachine::_sequenceStart))
    {
      // 2 seconds before moving to next sequence close all valves - to avoid simultaneous active polarity (both pins on HIGH state)
      _vp.deactivateValves(0);
    }
  }

  StateMachine::checkPump();
}

/**
   is program change allowed (only in first 2A phase)
*/
boolean StateMachine::isProgramChangeAllowed()
{
  return StateMachine::runningPhase == Constants::Phase::Filtration;
}

/**
   is wash allowed (only in 2A phase)
*/
boolean StateMachine::isWashAllowed()
{
  return StateMachine::runningPhase == Constants::Phase::Filtration;
}

/**
   deactive all valves. Set PIN state to LOW
*/
void StateMachine::deactivateValves()
{
  // deactivate all valves
  _vp.deactivateValves(500);
}

// ------------------
// Private methods

/*
  start cycle
*/
void StateMachine::start(const StateMachine::Cycle &cycle)
{
  StateMachine::_sequenceNumber = 0;
  StateMachine::_sequenceStart = now();

  // only if program was interrupted by wash cycle and wash cycle is finished
  if (StateMachine::_programSequence != -1 &&
      StateMachine::_programSequenceDuration != -1 &&
      cycle.cycleId != 0)
  {
    // set sequence position from last position
    StateMachine::_sequenceNumber = StateMachine::_programSequence;
    // add duration to last sequence duration
    StateMachine::_sequenceStart = now() - StateMachine::_programSequenceDuration;

    // reset all
    StateMachine::_programSequence = -1;
    StateMachine::_programSequenceDuration = -1;
  }

  // save current cycle for later process
  StateMachine::_currentCycle = cycle;
  // save current phase
  StateMachine::saveRunningPhase();
  // check pump whether pump must be turned on or off
  StateMachine::checkPump();
  // switch to new phase
  StateMachine::_vp.switchToPhase(StateMachine::runningPhase);
}

/*
  move to next sequence of cycle
*/
void StateMachine::moveToNextSequence()
{
  int cycleSequences = _currentCycle.sequences.size();
  if (cycleSequences > ++StateMachine::_sequenceNumber)
  {
    // reset sequence timer
    _sequenceStart = now();
    // save current phase
    StateMachine::saveRunningPhase();
    // check pump whether pump must be turned on or off
    StateMachine::checkPump();
    // switch to new phase
    _vp.switchToPhase(StateMachine::runningPhase);
  }
  else
  {
    if (_currentCycle.cycleId == 0) // Wash cycle ends
    {
      // turn off wash light after washing cycle
      digitalWrite(Constants::WashLight, LOW);
      // set running program after wash program
      StateMachine::runningProgram = StateMachine::programToRunAfterWash;
    }

    // set program step once everything is completed
    if (_currentCycle.nextCycleId != -1)
    {
      // 4 specified cycles
      for (int i = 0; i < 5; i++)
      {
        if (StateMachine::_cycles[i].cycleId == _currentCycle.nextCycleId)
        {
          StateMachine::start(StateMachine::_cycles[i]);
          break;
        }
      }
    }
    else
    {
      StateMachine::runningProgram = Constants::Program::ProgramNone;
    }
  }

  // turn on / off wash light
  digitalWrite(Constants::WashLight, 
    (StateMachine::runningPhase != Constants::Phase::Filtration &&
    StateMachine::runningPhase != Constants::Phase::Desinfection &&
    StateMachine::runningPhase != Constants::Phase::Close) ? HIGH : LOW);
}

/**
   checks whether pump must be turned on or off
   pump is turned on after 3 seconds
   pump is turned off on desinfection phase or 2AA phase
*/
void StateMachine::checkPump()
{
  // turn pump off during 2AA phase (backwash rusco) or desinfection phase and close phase
  // during 2AA sub phase (backwash rusco 2): turn off pump after 5 seconds and turn on pump after 10 seconds 
  boolean turnOffPumpBackwashRusco2 = 
    StateMachine::runningPhase == Constants::Phase::BackwashRusco2 &&
    (now() > (5 + _sequenceStart)) && 
    (now() < (11 + _sequenceStart));
  
  if (turnOffPumpBackwashRusco2 ||
      StateMachine::runningPhase == Constants::Phase::Desinfection ||
      StateMachine::runningPhase == Constants::Phase::Close)
  {
    if (_vp.pumpRunning)
    {
      digitalWrite(Constants::PumpLight, LOW);
      digitalWrite(Constants::UVLight, LOW);
      _vp.pumpRunning = false;
    }

    return;
  }

  if (!_vp.pumpRunning)
  {
    // turn pump on after 3 seconds
    if (StateMachine::runningPhase == Constants::Phase::FlushingSFRusco &&
        ((now() - _sequenceStart) < 3))
    {
      return;
    }

    digitalWrite(Constants::PumpLight, HIGH);
    _vp.pumpRunning = true;

    // turn on UV light after 10 seconds of backwash rusco 2 sub-phase
    if (StateMachine::runningPhase == Constants::Phase::BackwashRusco2 && (now() > (10 + _sequenceStart))) 
    {
      digitalWrite(Constants::UVLight, HIGH);
    }
  }
}

/*
  get sequence duration in seconds
*/
int StateMachine::getSequenceDuration()
{
  return _currentCycle.sequences[_sequenceNumber].duration;
}

/**
   save running phase
*/
void StateMachine::saveRunningPhase()
{
  StateMachine::runningPhase = _currentCycle.sequences[_sequenceNumber].phase;
}

/**
  init cycles (by default Program1 is selected)
*/
void StateMachine::init()
{
  // Cycle 1 - WASH
  StateMachine::Cycle cycle1;
  cycle1.cycleId = 0;
  cycle1.nextCycleId = 1;

  CycleSequence sequence;

  // State: Flushing SF (Rusco)
  sequence.phase = Constants::Phase::FlushingSFRusco;
  sequence.duration = Constants::FlushingSFRuscoDuration;
  sequence.canInterrupt = false;
  cycle1.sequences.push_back(sequence);

  // State: Flushing UF
  sequence.phase = Constants::Phase::FlushingUF1B;
  sequence.duration = Constants::FlushingUF1BDuration;
  sequence.canInterrupt = false;
  cycle1.sequences.push_back(sequence);

  // State: Flushing GAC
  sequence.phase = Constants::Phase::FlushingGAC;
  sequence.duration = Constants::FlushingGACDuration;
  sequence.canInterrupt = false;
  cycle1.sequences.push_back(sequence);

  StateMachine::_cycles[0] = cycle1;

  // Cycle 2 - FILTRATION
  // State: Filtration and BackwashRusco
  StateMachine::Cycle cycle2;
  cycle2.cycleId = 1;
  cycle2.nextCycleId = 2;

  // sequences to this cycle will be added when new program will be started
  StateMachine::_cycles[1] = cycle2;

  // Cycle 3 - WASH after FILTRATION
  StateMachine::Cycle cycle3;
  cycle3.cycleId = 2;
  cycle3.nextCycleId = 1;

  // State: Flushing UF
  sequence.phase = Constants::Phase::FlushingUF2B;
  sequence.duration = Constants::FlushingUF2BDuration;
  sequence.canInterrupt = false;
  cycle3.sequences.push_back(sequence);

  // State: Backwash UF 1
  sequence.phase = Constants::Phase::BackwashUF1;
  sequence.duration = Constants::BackwashUF1Duration;
  sequence.canInterrupt = false;
  cycle3.sequences.push_back(sequence);

  // State: Backwash UF 2
  sequence.phase = Constants::Phase::BackwashUF2;
  sequence.duration = Constants::BackwashUF2Duration;
  sequence.canInterrupt = false;
  cycle3.sequences.push_back(sequence);

  // State: Backwash UF 3
  sequence.phase = Constants::Phase::BackwashUF3;
  sequence.duration = Constants::BackwashUF3Duration;
  sequence.canInterrupt = false;
  cycle3.sequences.push_back(sequence);

  // State: Backwash UF 4
  sequence.phase = Constants::Phase::BackwashUF4;
  sequence.duration = Constants::BackwashUF4Duration;
  sequence.canInterrupt = false;
  cycle3.sequences.push_back(sequence);

  // State: Flushing UF
  sequence.phase = Constants::Phase::FlushingUF2G;
  sequence.duration = Constants::FlushingUF2GDuration;
  sequence.canInterrupt = false;
  cycle3.sequences.push_back(sequence);

  StateMachine::_cycles[2] = cycle3;

  // Desinfection
  // Phase: desinfection
  StateMachine::Cycle cycle4;
  cycle4.cycleId = 3;
  cycle4.nextCycleId = -1;

  sequence.phase = Constants::Phase::Desinfection;
  sequence.duration = Constants::DesinfectionDuration;
  sequence.canInterrupt = false;
  cycle4.sequences.push_back(sequence);

  StateMachine::_cycles[3] = cycle4;

  // Close
  // Phase: close
  StateMachine::Cycle cycle5;
  cycle5.cycleId = 4;
  cycle5.nextCycleId = -1; // state machine will stop executing all steps after 10 seconds (close duration)

  sequence.phase = Constants::Phase::Close;
  sequence.duration = Constants::CloseDuration;
  sequence.canInterrupt = false;
  cycle5.sequences.push_back(sequence);

  StateMachine::_cycles[4] = cycle5;
}

/**
   set filtration sequences - each program has different number of filtration sequences
   filtratrion sequences is combined with Filtration phase and Backwashrusco phase
*/
void StateMachine::setFiltrationSequences(Constants::Program program)
{
  if (StateMachine::_cycles[1].sequences.size() > 0)
  {
    StateMachine::_cycles[1].sequences.clear();
  }
  
  CycleSequence sequence;
  unsigned int duration = Constants::Program1Duration;
  if (program == Constants::Program::Program2)
  {
    duration = Constants::Program2Duration;
  }
  else if (program == Constants::Program::Program3)
  {
    duration = Constants::Program3Duration;
  }

  int sum = 0;
  while (sum < duration)
  {
    if (sum + Constants::FiltrationDuration >= duration)
    {
      sequence.phase = Constants::Phase::Filtration;
      sequence.duration = duration - sum;
      sequence.canInterrupt = true;
      StateMachine::_cycles[1].sequences.push_back(sequence);
      
      break;
    }
    
    sequence.phase = Constants::Phase::Filtration;
    sequence.duration = Constants::FiltrationDuration;
    sequence.canInterrupt = true;
    sum += Constants::FiltrationDuration;
    StateMachine::_cycles[1].sequences.push_back(sequence);

    sequence.phase = Constants::Phase::BackwashRusco1;
    sequence.duration = Constants::BackwashRusco1Duration;
    sequence.canInterrupt = false;
    sum += Constants::BackwashRusco1Duration;
    StateMachine::_cycles[1].sequences.push_back(sequence);

    sequence.phase = Constants::Phase::BackwashRusco2;
    sequence.duration = Constants::BackwashRusco2Duration;
    sequence.canInterrupt = false;
    sum += Constants::BackwashRusco2Duration;
    StateMachine::_cycles[1].sequences.push_back(sequence);
  }
}


