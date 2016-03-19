#ifndef Constants_h
#define Constants_h

#include "Arduino.h"

/**
  Program constants
*/
class Constants
{
  public:
    // valves constants
    const static int Valve1P = 35;
    const static int Valve1M = 37;
    const static int Valve2P = 31;
    const static int Valve2M = 33;
    const static int Valve3P = 27;
    const static int Valve3M = 29;
    const static int Valve4P = 23;
    const static int Valve4M = 25;
    const static int Valve5P = 20;
    const static int Valve5M = 21;
    const static int Valve6P = 39;
    const static int Valve6M = 41;
    const static int Valve7P = 43;
    const static int Valve7M = 45;
    const static int Valve8P = 47;
    const static int Valve8M = 49;
    const static int Valve9P = 51;
    const static int Valve9M = 53;
    
    const static int Valve10P = 13;
    const static int Valve10M = 12;
    const static int Valve11P = 11;
    const static int Valve11M = 3;
    const static int Valve12P = 2;
    const static int Valve12M = 42;
    const static int Valve13P = 44;
    const static int Valve13M = 46;
    const static int Valve14P = 48;
    const static int Valve14M = 50;

    // light indicators constants
    const static int PowerOnLight = 22;
    const static int Program1Light = 24;
    const static int Program2Light = 26;
    const static int Program3Light = 28;
    const static int DesinfectionLight = 30;
    const static int WashLight = 32;
    // light indicators and triggers constants
    const static int PumpLight = 19;
    const static int UVLight = 10;

    // Washing in filtration phases
    enum Phase
    {
      // 1A
      FlushingSFRusco = 0,
      // 1B
      FlushingUF1B = 1,
      // 1C
      FlushingGAC = 2,
      // 2A
      Filtration = 3,
      // 2AA
      BackwashRusco = 4,
      // 2B
      FlushingUF2B = 5,
      // 2C
      BackwashUF1 = 6,
      // 2D
      BackwashUF2 = 7,
      // 2E
      BackwashUF3 = 8,
      // 2F
      BackwashUF4 = 9,
      // 2G
      FlushingUF2G = 10,
      // Desinfection
      Desinfection = 11,
      // Close
      Close = 12
    };

    // Programs
    enum Program
    {
      Program1 = 0,
      Program2 = 1,
      Program3 = 2,
      ProgramDesinfection = 3,
      ProgramWash = 4,
      ProgramClose = 5,
      ProgramNone = 6
    };

    // Phase names (to display on screen while program is running)
    static char* PhaseNames[];
    // Program names (to display on screen while program is running)
    static char* ProgramNames[];

    // ---------------
    // Program durations
    const static int Program1Duration = 900;
    const static int Program2Duration = 1800;
    const static int Program3Duration = 3600;

    // ---------------
    // Phase durations
    const static int FlushingSFRuscoDuration = 30; // 30
    const static int FlushingUF1BDuration = 60; // 60
    const static int FlushingGACDuration = 60; // 60
    const static int FiltrationDuration = 300;
    const static int BackwashRuscoDuration = 300;
    const static int FlushingUF2BDuration = 60;
    const static int BackwashUF1Duration = 60;
    const static int BackwashUF2Duration = 60;
    const static int BackwashUF3Duration = 60;
    const static int BackwashUF4Duration = 60;
    const static int FlushingUF2GDuration = 60;
    const static int DesinfectionDuration = -1;
    const static int CloseDuration = 10;
    /*
      const static int FlushingSFRuscoDuration = 10;
      const static int FlushingUF1BDuration = 10;
      const static int FlushingGACDuration = 10;
      const static int FiltrationDuration = 30;
      const static int BackwashRuscoDuration = 30;
      const static int FlushingUF2BDuration = 30;
      const static int BackwashUF1Duration = 30;
      const static int BackwashUF2Duration = 30;
      const static int BackwashUF3Duration = 30;
      const static int BackwashUF4Duration = 30;
      const static int FlushingUF2GDuration = 30;
      const static int DesinfectionDuration = -1; // desinfection duration never ends
      const static int CloseDuration = 10;
    */
};

#endif
