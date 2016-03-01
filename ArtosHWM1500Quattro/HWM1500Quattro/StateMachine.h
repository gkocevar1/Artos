#ifndef StateMachine_h
#define StateMachine_h

#include "Arduino.h"
#include "Constants.h"
#include "HWM1500Quattro_debug.h" 
#include "ValvePhase.h"
#include <Vector2.h>
#include <Time.h>

class StateMachine
{
	public:
		/**
		* constructor
		*/
		StateMachine();

		/**
		* run program
		*
		* @param1: program to run
		*/
		void runProgram(Constants::Program);

		/*
		Check current cycle progress
		*/
		void checkProgress();
   
		// Cycle sequence. Each cycle has different cycle sequences. 
		// Each cycle sequence has phase, duration and flag which indicates whether cycle sequence can be interruptable or not
    // Interrupt means new program can be selected
		struct CycleSequence
		{
			// phase to execute
			Constants::Phase phase;

			// cycle duration (in seconds)
			unsigned int duration;

			// can interrupt current cycle 
			// false: sequence must execute completely
      // true: sequence can be interrupted
			boolean canInterrupt;
		};

		// Cycle. Each cycle contains cycle sequences.
		struct Cycle
		{
      // cycle identifier (counter is started with 0)
      unsigned int cycleId;

      // id of next cycle when all sequences are completed in current cycle 
      // -1 means that there is no cycle to execute after this cycle ends
      unsigned int nextCycleId;
    
			// sequences in the cycle
			Vector<CycleSequence> sequences;
		};

    Constants::Program runningProgram;
    Constants::Phase runningPhase;

	private:

		// State machine cycles (predefined cycles)
    // 0: WASH (Init)
    // 1: FILTRATION (Program1 or Program2 or Program3 with Backwash Rusco)
    // 2: BACKWASH (after each program)
    // 3: DESINFECTION
		Cycle _cycles[4];
    // Valve phase
    ValvePhase _vp;
    // Current cycle
    Cycle _currentCycle;
    // Current sequence number of a cycle
    int _sequenceNumber = 0;
    // Sequecen start time
    unsigned long _sequenceStart = 0;

		/**
		* init cycles
		*/
		void init();

    /**
     * set filtration sequences - each program has different number of filtration sequences
     * filtratrion sequences is combined with Filtration phase and Backwashrusco phase
     */
    void setFiltrationSequences(Constants::Program);

		/**
		* start cycle
		*/
		void start(const Cycle &);

		/*
		move to next sequence
		*/
		void moveToNextSequence();

		/*
		get sequence duration in seconds
		*/
		unsigned long getSequenceDuration();

    /**
     * save running phase
     */
    void saveRunningPhase();
};

#endif
