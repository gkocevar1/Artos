#ifndef ValvePhase_h
#define ValvePhase_h

#include "Arduino.h"
#include "Constants.h"
#include "HWM1500Quattro_debug.h" 

class ValvePhase
{
	public:

    /**
    * constructor
    */
    ValvePhase();
  
		/*
		Switch valves to desired phase
		*/
		void switchToPhase(Constants::Phase);

    /**
     * deactive all valves. Set PIN state to LOW
     */
    void deactivateValves();

    /**
     * Valve is combination of positive and negative state (or trigger). Two different states are controlled by two pins
     * 
     * At the time only one position is active (defined by active trigger). If active trigger is equals to -1 then valve is not controlled.
     */
    /*struct Valve
    {
      int positiveState;
      int negativeState;
      int activeState; 
    };*/
    
	private:

    /**
     * Init valves
     */
    void init();
    
		/*
		switch valves to proper position
		*/
		void switchValves(boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean);

    /**
     * activate valve
     * 
     *  @param1: valve id
     *  @param2: position of valve in array
     */
    void activateValve(int, int);

		/*
		Build info text
		*/
		String info(boolean);

    // Valve definitions
    //Valve _valves[14];
    // or
    // List of active valves - valves are closed two second before sequence ends
    int _activeValves[14];

		// flag indicates whether pump is running or not
		boolean _pumpRunning;
};

#endif
