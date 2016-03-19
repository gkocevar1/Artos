#ifndef ProgramConstants_h
#define ProgramConstants_h

#include "Arduino.h"

class ProgramConstants
{
  public:
    // Valves constants
    const static int _valve1P = 35;
    const static int _valve1M = 37;
    const static int _valve2P = 31;
    const static int _valve2M = 33;
    const static int _valve3P = 27;
    const static int _valve3M = 29;
    const static int _valve4P = 23;
    const static int _valve4M = 25;
    const static int _valve5P = 20;
    const static int _valve5M = 21;
    const static int _valve6P = 39;
    const static int _valve6M = 41;
    const static int _valve7P = 43;
    const static int _valve7M = 45;
    const static int _valve8P = 47;
    const static int _valve8M = 49;
    const static int _valve9P = 51;
    const static int _valve9M = 53;
    // LED light PIN constants
    const static int _powerOnLight = 22;
    const static int _program1Light = 24;
    const static int _program2Light = 26;
    const static int _program3Light = 28;
    const static int _desinfectionLight = 30;
    const static int _flushingLight = 32;
    // UV light
    const static int _UVLight = 19;
    // error light
    const static int _errorLight = 38;
    // service light
    const static int _serviceLight = 40; 
    // pump light
    const static int _pumpLight = 10;
};

#endif
