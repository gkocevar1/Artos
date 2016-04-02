#include "Constants.h"

char* Constants::PhaseNames[] =
{
  "Flushing SF",    //0  - FlushingSFRusco - 1A
  "Flushing UF",    //1  - FlushingUF1B - 1B
  "Flushing GAC",   //2  - FlushingGAC - 1C
  "Filtration",     //3  - Filtration - 2A
  "Backwash Rusco", //4  - BackwashRusco - 2AA
  "Flushing UF",    //5  - FlushingUF2B - 2B
  "Backwash UF1",   //6  - BackwashUF1 - 2C
  "Backwash UF2",   //7  - BackwashUF2 - 2D
  "Backwash UF3",   //8  - BackwashUF3 - 2E
  "Backwash UF4",   //9  - BackwashUF4 - 2F
  "Flushing UF",    //10 - FlushingUF2G - 2G
  "Desinfection",   //11 - Desinfection
  "Closing valves"           //12 - Close
};

char* Constants::ProgramNames[] =
{
  "Program: 1", //0 - Program1
  "Program: 2", //1 - Program2
  "Program: 3", //2 - Program3
  "Program: Desinf.", //3 - ProgramDesinfection
  "Program: Wash", //4 - Wash
  "Program: Desinf." // 5 - Close part of program desinfection
};



