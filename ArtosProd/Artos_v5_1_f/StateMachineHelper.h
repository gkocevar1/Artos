#ifndef StateMachineHelper_h
#define StateMachineHelper_h

#include "Arduino.h"

class StateMachineHelper
{
  public:  
    struct StateMachineResult
    {
      char *firstLineResult;
      char *secondLineResult;
      boolean stopEverything;
    };
    
    // flag (if true) indicates whether program can be selected with-in current running cycle
    boolean _allowNewProgramSelection;
    // flag (if true) indicates that next sequence of a cycle will be executed when button select is pressed
    boolean _waitForManuallTrigger;
    
    StateMachineHelper();
    StateMachineResult runCycle(int);
    StateMachineResult processCycle();
    StateMachineResult stopCycle();
    
    // set program state
    void setProgramState(int);
    // get program state
    int getProgramState();
    // set running cycle sequence id
    void setRunningCycleSequenceId(int);
    // blink light
    unsigned long blinkLigth(unsigned long, int);
    // switch all program lights
    void switchAllProgramLights(int);
    // wash is selected manually
    void activateWash();
  private:
    void runProgram(int);
    void displayRunningProgram();
    void checkCycleStatus();
    void updateStartUpSequence(int, boolean);
    void updateProgramSequence(int);
    void updateWashSequence(int, boolean);
    void updateDesinfectionSequence(int, boolean);
    void processProgramEnd();
    void stopPrivate();
    void printToFirstLine(char *);
    void printToSecondLine(char *);
    StateMachineResult setResult();
};

#endif
