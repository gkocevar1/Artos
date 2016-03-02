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
    * @param2: flag indicates if method is called when machine is turned on. In that case wash program is selected by default and program 1 must be selected as second cycle.
		*/
		void runProgram(Constants::Program, boolean);

		/**
    * Check current cycle progress
		*/
		void checkProgress();

    /**
     * is program change allowed (only in fist 2A phase)
     */
    boolean isProgramChangeAllowed();

    /**
     * is wash allowed (only in 2A phase)
     */
    boolean isWashAllowed();
   
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

    // Fields
    Constants::Program runningProgram;
    Constants::Phase runningPhase;

	private:

		// State machine cycles (predefined cycles)
    // 0: WASH (Init)
    // 1: FILTRATION (Program1 or Program2 or Program3 with Backwash Rusco)
    // 2: BACKWASH (after each program)
    // 3: DESINFECTION
    // 4: CLOSE (only after desinfection)
		Cycle _cycles[5];
    // Valve phase
    ValvePhase _vp;
    // Current cycle
    Cycle _currentCycle;
    // Current sequence number of a cycle
    int _sequenceNumber = 0;
    // Sequence start time
    unsigned long _sequenceStart = 0;

    // flag indicates whether first filtration (2A) phase is already executed or not. if not and current phase is filtration phase, only then change of program is allowed.
    boolean _isFirst2APhaseExecuted = false;
    
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

    /**
     * checks whether pump must be turned on or off
     * pump is turned on after 3 seconds
     * pump is turned off on desinfection phase or 2AA phase
     */
    void checkPump();
};

#endif
