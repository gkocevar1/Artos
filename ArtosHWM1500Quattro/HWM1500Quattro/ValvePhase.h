#ifndef ValvePhase_h
#define ValvePhase_h

#include "Arduino.h"
#include "Constants.h"
#include "HWM1500Quattro_debug.h" 

class ValvePhase
{
  public:

    /**
     * Switch valves to desired phase
     */
    void switchToPhase(Constants::Phase);
    
  private:

    /**
     * switch valves to proper position
     */
    void switchValves(boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean);

    /**
     * Only to build info string - not needed in PRODUCTION
     */
    String info(boolean);
    
    // flag indicates whether pump is running or not
    boolean _pumpRunning;
};

#endif
