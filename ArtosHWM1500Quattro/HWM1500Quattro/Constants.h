#ifndef Constants_h
#define Constants_h

#include "Arduino.h"

/**
 * Program constants
 */
class Constants
{
  public:
    // valves constants
    const static int Valve1P = 20;
    const static int Valve1M = 21;
    const static int Valve2P = 22;
    const static int Valve2M = 23;
    const static int Valve3P = 24;
    const static int Valve3M = 25;
    const static int Valve4P = 26;
    const static int Valve4M = 27;
    const static int Valve5P = 28;
    const static int Valve5M = 29;
    const static int Valve6P = 30;
    const static int Valve6M = 31;
    const static int Valve7P = 32;
    const static int Valve7M = 33;
    const static int Valve8P = 34;
    const static int Valve8M = 35;
    const static int Valve9P = 36;
    const static int Valve9M = 37;
    const static int Valve10P = 38;
    const static int Valve10M = 39;
    const static int Valve11P = 40;
    const static int Valve11M = 41;
    const static int Valve12P = 42;
    const static int Valve12M = 43;
    const static int Valve13P = 44;
    const static int Valve13M = 45;
    const static int Valve14P = 46;
    const static int Valve14M = 47;
    
    // light indicator constants
    const static int PowerOnLight = 14;
    const static int Program1Light = 15;
    const static int Program2Light = 16;
    const static int Program3Light = 17;
    
    /*const static int DesinfectionLight = 18;
    const static int FlushingLight = 19;
    // UV light
    const static int UVLight = 48;
    // error light
    const static int ErrorLight = 49;
    // service light
    const static int ServiceLight = 50; 
    // pump light
    const static int PumpLight = 51;*/

    // Washing in filtration phases
    enum Phase
    { 
      FlushingSFRusco = 0, 
      FlushingUF = 1, 
      FlushingGAC = 2, 
      Filtration = 3, 
      BackwashRusco = 4, 
      BackwashUF1 = 5, 
      BackwashUF2 = 6, 
      BackwashUF3 = 7, 
      BackwashUF4 = 8,
      Desinfection = 9,
      Close = 10
    };

    // Program and phase durations (in seconds)
    const static int Program1Duration = 900; 
    const static int Program2Duration = 1800; 
    const static int Program3Duration = 3600; 
    const static int DesinfectionDuration = 30; 
    
    const static int FlushingSFRuscoDuration = 30; 
    const static int FlushingUFDuration = 60; 
    const static int FlushingGACDuration = 60;
    const static int BackwashRuscoDuration = 300; 
    const static int BackwashUF1Duration = 60; 
    const static int BackwashUF2Duration = 60; 
    const static int BackwashUF3Duration = 60; 
    const static int BackwashUF4Duration = 60;
};

#endif
