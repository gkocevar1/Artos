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
    const static int UVLight = 18; //10;

    // Washing in filtration phases
    // Backwash rusco phase is splitted to two (sub)phase 
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
      BackwashRusco1 = 4,
      BackwashRusco2 = 5,
      // 2B
      FlushingUF2B = 6,
      // 2C
      BackwashUF1 = 7,
      // 2D
      BackwashUF2 = 8,
      // 2E
      BackwashUF3 = 9,
      // 2F
      BackwashUF4 = 10,
      // 2G
      FlushingUF2G = 11,
      // Desinfection
      Desinfection = 12,
      // Close
      Close = 13
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
    const static int FlushingSFRuscoDuration = 5; // 30
    const static int FlushingUF1BDuration = 5; // 60
    const static int FlushingGACDuration = 5; // 60
    const static int FiltrationDuration = 20; // 300
    
    // Backwash rusco duration is 22 seconds - phase is splitted into two subphases
    const static int BackwashRusco1Duration = 7; // 7
    const static int BackwashRusco2Duration = 15; // 15
    
    const static int FlushingUF2BDuration = 10;
    const static int BackwashUF1Duration = 10;
    const static int BackwashUF2Duration = 10;
    const static int BackwashUF3Duration = 10;
    const static int BackwashUF4Duration = 10;
    const static int FlushingUF2GDuration = 10;
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
