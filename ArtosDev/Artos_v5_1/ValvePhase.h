#ifndef ValvePhase_h
#define ValvePhase_h

#include "Arduino.h"

class ValvePhase
{
  public:
  
    // whitch valves to desired position
    void switchValvesPosition(int);
    // check valves status and stop trigger (triggers are stopped after 10 seconds)
    void checkValveTriggersStatus(unsigned long);
    
    void forceStop();
  private:
    // 1. Zagon
    void switchStartupValvesPosition();
    
    // 3. Odzracevanje UF
    void switchVentValvesPosition();
    
    // 4. P. spiranje A
    void switchPFlushingAValvesPosition();
    
    // 5. P. spiranje B
    void switchPFlushingBValvesPosition();
    
    // 6. Delovanje UF + spiranje filtra
    void switchWorkingUFValvesPosition();
    
    // 7. Normalno delovanje
    void switchNormalValvesPosition();
    
    // 8. Delovanje
    void switchWorkingValvesPosition();
    
    // 9. ciscenje Spiranje A
    void switchCleaningFlushingAValvesPosition();
    
    // 10. ciscenje P. spiranje A
    void switchCleaningPFlushingAValvesPosition();
    
    // 11. ciscenje Spiranje B
    void switchCleaningFlushingBValvesPosition();
    
    // 12. ciscenje P. spiranje B
    void switchCleaningPFlushingBValvesPosition();
    
    // 13. dezinfekcija
    void switchDesValvesPosition();
    
    // 14. koncana dezinfekcija
    void switchEndDesValvesPosition();
    
    // 15. zapiranje ventilov
    void switchCloseValvesPosition();
    
    // switch valves
    void switchValves(boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean);
    
    // turn on valve trigger
    void activateValveTrigger(int, unsigned long);
    
    // flag indicates whether pump is running or not
    boolean _pumpRunning;
};

#endif


