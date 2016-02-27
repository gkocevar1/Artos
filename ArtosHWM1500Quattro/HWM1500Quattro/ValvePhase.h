#ifndef ValvePhase_h
#define ValvePhase_h

#include "Arduino.h"
#include "Constants.h"

class ValvePhase
{
  public:

    /**
     * Switch valves to desired phase
     */
    void switchToPhase(Constants::Phase);
    
  private:

    /**
     * flushing SF (Rusco)
     */
    void flushingSFRusco();

    /**
     * flushing both UF
     */
    void flushingUF();

    /**
     * flushing GAC
     */
    void flushingGAC();

    /**
     * filtration
     */
    void filtration();

    /**
     * backwash Rusco 
     */
    void backwashRusco();

    /**
     * backwash UF 1
     */
    void backwashUF1();

    /**
     * backwash UF 2
     */
    void backwashUF2();

    /**
     * backwash UF 3
     */
    void backwashUF3();

    /**
     * backwash UF 4
     */
    void backwashUF4();

    /**
     * desinfection
     */
    void desinfection();

    /**
     * close all valves
     */
    void closeValves();

    /**
     * switch valves to proper position
     */
    void switchValves(boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean);
    
    // flag indicates whether pump is running or not
    boolean _pumpRunning;
};

#endif
