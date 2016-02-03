#ifndef StateMachine_h
#define StateMachine_h

#include "Arduino.h"

class StateMachine
{
  public:  
    StateMachine();
  
    // special cycle flags
    const static int _displayRunningCycle = 97;
    const static int _manullyToNextSequence = 98;
    const static int _cycleEnd = 99;
  
    int runProgramCycle(int);
    // check running program cycle (cycleId, sequenceId)
    int checkProgramCycleProgress(int, int);  
    // check if cycle is interruptable (program1, program2, program3, desinfection programs can be interrupted)
    boolean isCycleInterruptable(int);  
    // gets the end cycle id
    int getEndCycleId(int);
    // indicates whether to repeat last cycle
    boolean repeatLastCycle(int);
    // check if interruption is in progress - in-program interruption (program 1, 2, 3)
    boolean hasInterruption();
    // check if program allows interruption
    boolean allowNewProgramSelection(int);
    // blink light
    unsigned long blinkLigth(unsigned long, int);
    
  private:

    struct CycleSequence
    {
      int sequenceId;
      unsigned long duration;
      int valvePhaseId;
    };
    
    struct InterruptCycleSequence
    {
      int repeatAfterSeconds; // in seconds
      unsigned long duration; // in seconds
      int valvePhaseId;
    };
    
    struct Cycle
    {
      int cycleId;  // cycle id
      boolean repeatAfterEnd; // repeat cycle when cycle ends
      boolean allowCycleInterruption; // if cycle is running then allow interruption (manual interruption)
      boolean hasInProgramInterruption; // indicates whether cylce has in-program interruption (like pre-filter cleaning)
      int endCycleId; // this is used for: when we need to run special cycle when current cycle end
      boolean manuallyTriggered; // set to true if next sequence must be manually triggered
      int sequencesNo; // number of sequences in cycle
      CycleSequence sequence[6]; // cycle sequences
      InterruptCycleSequence interruptCycleSequence; // in-program interrupt cycle sequence (like pre-filter cleaning)
    };
    
    Cycle _cycles[7];
    
    const static int _program1Duration = 900; // 900 seconds
    const static int _program2Duration = 1800; // 1800 seconds
    const static int _program3Duration = 3600; // 3600 seconds
    const static int _desinfectionDuration = 30; // 30 seconds
    const static int _prefilterCleaningStart = 120; // pre-filter cleaning start after every 120 seconds
    const static int _prefilterCleaningDuration = 16; // pre-filter cleaning duration 16 seconds (6 seconds for valves to open and 10 seconds of filter pre-cleaning)
    const static int _cleaningPhaseDuration = 60; // change phase every 60 seconds (total 240 seconds (total 4 phases))
  
    int checkCycleProgress(Cycle, CycleSequence, int);
    boolean checkForInprogramInterruption(Cycle, CycleSequence, unsigned long);
    Cycle getCycle(int); 
    void initCycles(); 
    void updateValvesPositions(int);
};

#endif
