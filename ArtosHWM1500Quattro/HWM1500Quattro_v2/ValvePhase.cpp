#include "ValvePhase.h"

/**
   Constructor
*/
ValvePhase::ValvePhase()
{
  ValvePhase::init();
}

// ------------------
// Public methods

/**
  Switch valves to desired phase
*/
void ValvePhase::switchToPhase(Constants::Phase phase)
{
  //DMSG1("New Phase: "); DMSG(Constants::PhaseNames[phase]);

  switch (phase)
  {
    case Constants::Phase::FlushingSFRusco:
      {
        ValvePhase::switchValves(
          false, // 1
          false, // 2
          false, // 3
          false, // 4
          false, // 5
          true, // 6
          false, // 7
          false, // 8
          false, // 9
          false, // 10
          false, // 11
          false, // 12
          false, // 13
          false  // 14
        );
        break;
      }
    case Constants::Phase::FlushingUF1B:
    case Constants::Phase::FlushingUF2G:
      {
        ValvePhase::switchValves(
          true, // 1
          true, // 2
          true, // 3
          true, // 4
          false, // 5
          false, // 6
          false, // 7
          false, // 8
          false, // 9
          true, // 10
          true, // 11
          true, // 12
          true, // 13
          true  // 14
        );
        break;
      }
    case Constants::Phase::FlushingGAC:
      {
        ValvePhase::switchValves(
          true, // 1
          true, // 2
          false, // 3
          false, // 4
          true, // 5
          false, // 6
          true, // 7
          true, // 8
          false, // 9
          true, // 10
          true, // 11
          false, // 12
          false, // 13
          true  // 14
        );
        break;
      }
    case Constants::Phase::Filtration:
      {
        ValvePhase::switchValves(
          true, // 1
          true, // 2
          false, // 3
          false, // 4
          true, // 5
          false, // 6
          false, // 7
          false, // 8
          true, // 9
          true, // 10
          true, // 11
          false, // 12
          false, // 13
          true  // 14
        );
        break;
      }
    case Constants::Phase::BackwashRusco:
      {
        ValvePhase::switchValves(
          true, // 1
          true, // 2
          false, // 3
          false, // 4
          true, // 5
          true, // 6
          false, // 7
          false, // 8
          true, // 9
          true, // 10
          true, // 11
          false, // 12
          false, // 13
          false  // 14
        );
        break;
      }
    case Constants::Phase::BackwashUF1:
      {
        ValvePhase::switchValves(
          false, // 1
          true, // 2
          true, // 3
          false, // 4
          false, // 5
          false, // 6
          false, // 7
          false, // 8
          true, // 9
          true, // 10
          true, // 11
          false, // 12
          false, // 13
          true  // 14
        );
        break;
      }
    case Constants::Phase::BackwashUF2:
      {
        ValvePhase::switchValves(
          true, // 1
          false, // 2
          false, // 3
          true, // 4
          false, // 5
          false, // 6
          false, // 7
          false, // 8
          true, // 9
          true, // 10
          true, // 11
          false, // 12
          false, // 13
          true  // 14
        );
        break;
      }
    case Constants::Phase::BackwashUF3:
      {
        ValvePhase::switchValves(
          true, // 1
          true, // 2
          false, // 3
          false, // 4
          false, // 5
          false, // 6
          false, // 7
          false, // 8
          true, // 9
          false, // 10
          true, // 11
          true, // 12
          false, // 13
          true  // 14
        );
        break;
      }
    case Constants::Phase::BackwashUF4:
      {
        ValvePhase::switchValves(
          true, // 1
          true, // 2
          false, // 3
          false, // 4
          false, // 5
          false, // 6
          false, // 7
          false, // 8
          true, // 9
          true, // 10
          false, // 11
          false, // 12
          true, // 13
          true  // 14
        );
        break;
      }
    case Constants::Phase::Desinfection:
      {
        ValvePhase::switchValves(
          false, // 1
          false, // 2
          true, // 3
          true, // 4
          false, // 5
          false, // 6
          false, // 7
          true, // 8
          true, // 9
          false, // 10
          false, // 11
          true, // 12
          true, // 13
          false  // 14
        );
        break;
      }
    case Constants::Phase::FlushingUF2B:
      {
        ValvePhase::switchValves(
          true, // 1
          true, // 2
          true, // 3
          true, // 4
          false, // 5
          false, // 6
          false, // 7
          false, // 8
          true, // 9
          true, // 10
          true, // 11
          true, // 12
          true, // 13
          true  // 14
        );
        break;
      }
    case Constants::Phase::Close:
      {
        ValvePhase::switchValves(
          false, // 1
          false, // 2
          false, // 3
          false, // 4
          false, // 5
          false, // 6
          false, // 7
          false, // 8
          false, // 9
          false, // 10
          false, // 11
          false, // 12
          false, // 13
          false  // 14
        );
        break;
      }
  }
}

/**
   deactive all valves. Set PIN state to LOW

   @param1: delay after deactivation all valves (in milliseconds)
*/
void ValvePhase::deactivateValves(unsigned int delayTime)
{
  //DMSG1("ValvePhase::deactivateValves with delay: ");DMSG(delayTime);
  for (int i = 0; i < 14; i++)
  {
    if (_activeValves[i] != -1)
    {
      //DMSG1("D:");DMSG1(_activeValves[i]);DMSG1(" ");
      // deactivate valve
      digitalWrite(_activeValves[i], LOW);
      _activeValves[i] = -1;
    }
  }
  
  delay(delayTime);
}

// ------------------
// private methods

/**
  switch valves to proper position
*/
void ValvePhase::switchValves(boolean v1, boolean v2, boolean v3, boolean v4, boolean v5, boolean v6, boolean v7, boolean v8, boolean v9, boolean v10, boolean v11, boolean v12, boolean v13, boolean v14)
{
  /*String info = String(
    "v1:" + ValvePhase::info(v1) + ", " +
    "v2:" + ValvePhase::info(v2) + ", " +
    "v3:" + ValvePhase::info(v3) + ", " +
    "v4:" + ValvePhase::info(v4) + ", " +
    "v5:" + ValvePhase::info(v5) + ", " +
    "v6:" + ValvePhase::info(v6) + ", " +
    "v7:" + ValvePhase::info(v7) + ", " +
    "v8:" + ValvePhase::info(v8) + ", " +
    "v9:" + ValvePhase::info(v9) + ", " +
    "v10:" + ValvePhase::info(v10) + ", " +
    "v11:" + ValvePhase::info(v11) + ", " +
    "v12:" + ValvePhase::info(v12) + ", " +
    "v13:" + ValvePhase::info(v13) + ", " +
    "v14:" + ValvePhase::info(v14));
  DMSG(info);*/
 
  ValvePhase::activateValve((v1 ? Constants::Valve1P : Constants::Valve1M), 0);
  ValvePhase::activateValve((v2 ? Constants::Valve2P : Constants::Valve2M), 1);
  ValvePhase::activateValve((v3 ? Constants::Valve3P : Constants::Valve3M), 2);
  ValvePhase::activateValve((v4 ? Constants::Valve4P : Constants::Valve4M), 3);
  ValvePhase::activateValve((v5 ? Constants::Valve5P : Constants::Valve5M), 4);
  ValvePhase::activateValve((v6 ? Constants::Valve6P : Constants::Valve6M), 5);
  ValvePhase::activateValve((v7 ? Constants::Valve7P : Constants::Valve7M), 6);
  ValvePhase::activateValve((v8 ? Constants::Valve8P : Constants::Valve8M), 7);
  ValvePhase::activateValve((v9 ? Constants::Valve9P : Constants::Valve9M), 8);
  ValvePhase::activateValve((v10 ? Constants::Valve10P : Constants::Valve10M), 9);
  ValvePhase::activateValve((v11 ? Constants::Valve11P : Constants::Valve11M), 10);
  ValvePhase::activateValve((v12 ? Constants::Valve12P : Constants::Valve12M), 11);
  ValvePhase::activateValve((v13 ? Constants::Valve13P : Constants::Valve13M), 12);
  ValvePhase::activateValve((v14 ? Constants::Valve14P : Constants::Valve14M), 13);

  // UV light is turned on when pump is running and valve 9 is open.
  digitalWrite(Constants::UVLight, ((pumpRunning && v9) ? HIGH : LOW));
}

/**
   activate valve
*/
void ValvePhase::activateValve(int valveId, int pos)
{
  // save active valve
  _activeValves[pos] = valveId;
  // open valve
  digitalWrite(valveId, HIGH);
}

/**
   Init valves
*/
void ValvePhase::init()
{
  _activeValves[0] = -1;
  _activeValves[1] = -1;
  _activeValves[2] = -1;
  _activeValves[3] = -1;
  _activeValves[4] = -1;
  _activeValves[5] = -1;
  _activeValves[6] = -1;
  _activeValves[7] = -1;
  _activeValves[8] = -1;
  _activeValves[9] = -1;
  _activeValves[10] = -1;
  _activeValves[11] = -1;
  _activeValves[12] = -1;
  _activeValves[13] = -1;
}

/**
  Only to build info string - not needed in PRODUCTION
*/
String ValvePhase::info(boolean v)
{
  return v ? "open" : "close";
}
