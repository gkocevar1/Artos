#include "Arduino.h"
#include "ValvePhase.h"
#include <Time.h>

// ------------------
// Public methods

/**
 * Switch valves to desired phase
 */
void ValvePhase::ValvePhase::switchToPhase(Constants::Phase phase)
{
  switch (phase)
  {
    case Constants::Phase::FlushingSFRusco:
    {
      ValvePhase::flushingSFRusco();  
      break;
    }
    case Constants::Phase::FlushingUF:
    {
      ValvePhase::flushingUF();  
      break;
    }
    case Constants::Phase::FlushingGAC:
    {
      ValvePhase::flushingGAC();  
      break;
    }
    case Constants::Phase::Filtration:
    {
      ValvePhase::filtration();  
      break;
    }
    case Constants::Phase::backwashRusco:
    {
      ValvePhase::backwashRusco();  
      break;
    }
    case Constants::Phase::BackwashUF1:
    {
      ValvePhase::backwashUF1();  
      break;
    }
    case Constants::Phase::BackwashUF2:
    {
      ValvePhase::backwashUF2();  
      break;
    }
    case Constants::Phase::BackwashUF3:
    {
      ValvePhase::backwashUF3();  
      break;
    }
    case Constants::Phase::BackwashUF4:
    {
      ValvePhase::backwashUF4();  
      break;
    }
    case Constants::Phase::Desinfection:
    {
      ValvePhase::desinfection();  
      break;
    }
    case Constants::Phase::Close:
    {
      ValvePhase::closeValves();  
      break;
    }
  }
}

// ------------------
// private methods

/**
 * flushing SF (Rusco)
 */
void ValvePhase::flushingSFRusco()
{
  
}

/**
 * flushing both UF
 */
void ValvePhase::flushingUF()
{
  
}

/**
 * flushing GAC
 */
void ValvePhase::flushingGAC()
{
  
}

/**
 * filtration
 */
void ValvePhase::filtration()
{
  
}

/**
 * backwash Rusco 
 */
void ValvePhase::backwashRusco()
{
  
}

/**
 * backwash UF 1
 */
void ValvePhase::backwashUF1()
{
  
}

/**
 * backwash UF 2
 */
void ValvePhase::backwashUF2()
{
  
}

/**
 * backwash UF 3
 */
void ValvePhase::backwashUF3()
{
  
}

/**
 * backwash UF 4
 */
void ValvePhase::backwashUF4()
{
  
}

/**
 * desinfection
 */
void ValvePhase::desinfection()
{
  
}

/**
 * close all valves
 */
void ValvePhase::closeValves()
{
  
}

/**
 * switch valves to proper position
 */
void ValvePhase::switchValves(boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean, boolean)
{
  
}

