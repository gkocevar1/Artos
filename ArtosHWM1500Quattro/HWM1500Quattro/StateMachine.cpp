#include "StateMachine.h"

/**
* Constructor
*/
StateMachine::StateMachine()
{
	StateMachine::init();
}

// ------------------
// Public methods

/*
run selected program
*/
void StateMachine::runProgram(Constants::Program program)
{
  StateMachine::runningProgram = program;
  
	switch (program)
	{
		case Constants::Program::Program1:
		{
			DMSG("StateMachine::runProgram - Program1");
			// set sequences for program 1
      StateMachine::setFiltrationSequences(Constants::Program::Program1);
			StateMachine::start(StateMachine::_cycles[0]);
      
			break;
		}
		case Constants::Program::Program2:
		{
			DMSG("StateMachine::runProgram - Program2");
			// set sequences for program 2
      StateMachine::setFiltrationSequences(Constants::Program::Program2);
			StateMachine::start(StateMachine::_cycles[0]);

			break;
		}
		case Constants::Program::Program3:
		{
			DMSG("StateMachine::runProgram - Program3");
			// set sequences for program 3
      StateMachine::setFiltrationSequences(Constants::Program::Program3);
			StateMachine::start(StateMachine::_cycles[0]);

			break;
		}
		case Constants::Program::ProgramDesinfection:
		{
			DMSG("StateMachine::runProgram - Desinfection");
			
      
			break;
		}
   case Constants::Program::Wash:
   {
      DMSG("StateMachine::runProgram - Wash");
      
      break;
    }
	}

  //return result;
}

/*
check current cycle progress
*/
void StateMachine::checkProgress()
{
  //StateMachine::Result result;
	if (now() > (StateMachine::getSequenceDuration() + _sequenceStart))
	{
		// move to next sequence
		StateMachine::moveToNextSequence();
	}

  //return result;
}

// ------------------
// Private methods

/*
start cycle
*/
void StateMachine::start(const StateMachine::Cycle &cycle)
{
  DMSG("StateMachine::start - Start new cycle");

  // reset sequence number
  StateMachine::_sequenceNumber = 0;
  // reset sequence timer
  StateMachine::_sequenceStart = now();
  // switch to new phase
  StateMachine::_vp.switchToPhase(cycle.sequences[_sequenceNumber].phase);
  // save current cycle for later process
  StateMachine::_currentCycle = cycle;
  // save current phase
  StateMachine::saveRunningPhase();
}

/*
move to next sequence of cycle
*/
void StateMachine::moveToNextSequence()
{
  DMSG("StateMachine::moveToNextSequence");
  
  int cycleSequences = _currentCycle.sequences.size();
  if (cycleSequences > ++_sequenceNumber)
  {
    DMSG("Proceed to next sequence");
    
    _vp.switchToPhase(_currentCycle.sequences[_sequenceNumber].phase);
    _sequenceStart = now();

    // update current running phase
    StateMachine::saveRunningPhase();
  }
  else
  {
    // set program step once everything is completed
    DMSG("Proceed to next cycle");
    if (_currentCycle.nextCycleId != -1)
    {
      // 4 specified cycles
      for (int i = 0; i < 4; i++)
      {
        if (StateMachine::_cycles[i].cycleId == _currentCycle.nextCycleId)
        {
          DMSG("New cycle is founded");
          StateMachine::start(StateMachine::_cycles[i]);
          break;
        }
      }
    }
    else
    {
      DMSG("---STOP EVERYTHING");
    }
  }
}

/*
get sequence duration in milliseconds
*/
unsigned long StateMachine::getSequenceDuration()
{
  return _currentCycle.sequences[_sequenceNumber].duration;
}

/**
 * save running phase
 */
void StateMachine::saveRunningPhase()
{
  StateMachine::runningPhase = _currentCycle.sequences[_sequenceNumber].phase;
}

/**
* init cycles (by default Program2 is selected)
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
	sequence.canInterrupt = true;
	cycle1.sequences.push_back(sequence);

	// State: Flushing UF
	sequence.phase = Constants::Phase::FlushingUF1B;
	sequence.duration = Constants::FlushingUF1BDuration;
	sequence.canInterrupt = true;
	cycle1.sequences.push_back(sequence);

	// State: Flushing GAC
	sequence.phase = Constants::Phase::FlushingGAC;
	sequence.duration = Constants::FlushingGACDuration;
	sequence.canInterrupt = true;
	cycle1.sequences.push_back(sequence);

  StateMachine::_cycles[0] = cycle1;

  // Cycle 2 - Filtration
	// State: Filtration and BackwashRusco
  StateMachine::Cycle cycle2;
  cycle2.cycleId = 1;
  cycle2.nextCycleId = 2;

  // sequences to this cycle will be added when new program will be started
  StateMachine::_cycles[1] = cycle2;
  
  StateMachine::Cycle cycle3;
  cycle3.cycleId = 2;
  cycle3.nextCycleId = 1;

  // State: Flushing UF
  sequence.phase = Constants::Phase::FlushingUF2B;
  sequence.duration = Constants::FlushingUF2BDuration;
  sequence.canInterrupt = true;
  cycle3.sequences.push_back(sequence);

  // State: Backwash UF 1
  sequence.phase = Constants::Phase::BackwashUF1;
  sequence.duration = Constants::BackwashUF1Duration;
  sequence.canInterrupt = true;
  cycle3.sequences.push_back(sequence);

  // State: Backwash UF 2
  sequence.phase = Constants::Phase::BackwashUF2;
  sequence.duration = Constants::BackwashUF2Duration;
  sequence.canInterrupt = true;
  cycle3.sequences.push_back(sequence);

  // State: Backwash UF 3
  sequence.phase = Constants::Phase::BackwashUF3;
  sequence.duration = Constants::BackwashUF3Duration;
  sequence.canInterrupt = true;
  cycle3.sequences.push_back(sequence);

  // State: Backwash UF 4
  sequence.phase = Constants::Phase::BackwashUF4;
  sequence.duration = Constants::BackwashUF4Duration;
  sequence.canInterrupt = true;
  cycle3.sequences.push_back(sequence);

  // State: Flushing UF
  sequence.phase = Constants::Phase::FlushingUF2G;
  sequence.duration = Constants::FlushingUF2GDuration;
  sequence.canInterrupt = true;
  cycle3.sequences.push_back(sequence);

  StateMachine::_cycles[2] = cycle3;

  // Desinfection
  // Phase: desinfection
	StateMachine::Cycle cycle4;
  cycle4.cycleId = 3;
  cycle4.nextCycleId = -1; // state machine will stop executing all steps

  sequence.phase = Constants::Phase::Desinfection;
  sequence.duration = Constants::DesinfectionDuration;
  sequence.canInterrupt = false;
  cycle4.sequences.push_back(sequence);

  sequence.phase = Constants::Phase::Close;
  sequence.duration = Constants::CloseDuration;
  sequence.canInterrupt = false;
  cycle4.sequences.push_back(sequence);

  StateMachine::_cycles[3] = cycle4;
}

/**
 * set filtration sequences - each program has different number of filtration sequences
 * filtratrion sequences is combined with Filtration phase and Backwashrusco phase
 */
void StateMachine::setFiltrationSequences(Constants::Program program)
{
  if (StateMachine::_cycles[1].sequences.size() > 0)
  {
    StateMachine::_cycles[1].sequences.clear();
  }

  CycleSequence sequence;
  for (int i = 0; i < (program / 300); i++)
  {
    if (i % 2 == 0)
    {
      sequence.phase = Constants::Phase::Filtration;
      sequence.duration = Constants::FiltrationDuration;
      sequence.canInterrupt = true;
    }
    else
    {
      sequence.phase = Constants::Phase::BackwashRusco;
      sequence.duration = Constants::BackwashRuscoDuration;
      sequence.canInterrupt = true;
    }

    StateMachine::_cycles[1].sequences.push_back(sequence);
  }
}


