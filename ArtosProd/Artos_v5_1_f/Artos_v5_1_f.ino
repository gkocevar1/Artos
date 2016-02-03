#include <Time.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "EEPROMHelper.h"
#include "ProgramConstants.h"
#include "Helper.h"
#include "StateMachineHelper.h"


//
// PIN constants are defined in ProgramConstants.h
//

// button PIN constants
#define btnRIGHT  0 // flushing
#define btnUP     1 // service info
#define btnDOWN   2
#define btnLEFT   3 // program selection
#define btnSELECT 4 // confirmation button
#define btnNONE   5

// start time (timer starts when pump is switched on)
unsigned long _startTime = now();
// flag (if true) indicates that service is required
boolean _serviceRequired = false;
// flag (if true) indicates that service is required and no program can be selected
boolean _doNothing = false;
// flag (if true) indicates that one program is running
boolean _programRunning = false;
// service light last blink
unsigned long _serviceLight;
// flag (if true) indicates that pump is running
boolean _pumpRunning = false;
// flag (if true) indicates that machine is prepared to be turned off
boolean _preparedToSwitchOff = false;
// flag indicates that text is printed to the LCD screen
boolean _lastTextPrinted = true;

int _selected = 0;
int _programStateInternal = 0;

// last pressed button time (status / switch between programs and select button is not pressed then return to previous text after 5 s)
unsigned long _lastPressed = 0;

char* _aFirstLine;
char* _aSecondLine;
unsigned long _lastDisplayRefreshTime = -1;

// select the pins used on the LCD panel
//LiquidCrystal lcd(9, 8, 7, 6, 5, 4); //  novo  rs=d9 e=d8 D4=d7 D5=d6 D6=d5 D7=d4  , prej rs=8, e=9, D4=4, D=5, D=6, d7=7
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
Helper helper;
StateMachineHelper stateMachineHelper;

void update(StateMachineHelper::StateMachineResult);

void setup()
{
  // initialize outputs
  pinMode(ProgramConstants::_powerOnLight, OUTPUT);
  pinMode(ProgramConstants::_pumpLight, OUTPUT);
  pinMode(ProgramConstants::_errorLight, OUTPUT);
  pinMode(ProgramConstants::_program1Light, OUTPUT);
  pinMode(ProgramConstants::_program2Light, OUTPUT);
  pinMode(ProgramConstants::_program3Light, OUTPUT);
  pinMode(ProgramConstants::_desinfectionLight, OUTPUT);
  pinMode(ProgramConstants::_flushingLight, OUTPUT);
  pinMode(ProgramConstants::_UVLight, OUTPUT);
  pinMode(ProgramConstants::_serviceLight, OUTPUT);
  pinMode(ProgramConstants::_valve1P, OUTPUT);
  pinMode(ProgramConstants::_valve1M, OUTPUT);
  pinMode(ProgramConstants::_valve2P, OUTPUT);
  pinMode(ProgramConstants::_valve2M, OUTPUT);
  pinMode(ProgramConstants::_valve3P, OUTPUT);
  pinMode(ProgramConstants::_valve3M, OUTPUT);
  pinMode(ProgramConstants::_valve4P, OUTPUT);
  pinMode(ProgramConstants::_valve4M, OUTPUT);
  pinMode(ProgramConstants::_valve5P, OUTPUT);
  pinMode(ProgramConstants::_valve5M, OUTPUT);
  pinMode(ProgramConstants::_valve6P, OUTPUT);
  pinMode(ProgramConstants::_valve6M, OUTPUT);
  pinMode(ProgramConstants::_valve7P, OUTPUT);
  pinMode(ProgramConstants::_valve7M, OUTPUT);
  pinMode(ProgramConstants::_valve8P, OUTPUT);
  pinMode(ProgramConstants::_valve8M, OUTPUT);
  pinMode(ProgramConstants::_valve9P, OUTPUT);
  pinMode(ProgramConstants::_valve9M, OUTPUT);
  // turn on power light
  digitalWrite(ProgramConstants::_powerOnLight, HIGH);
  
  // init LCD screen
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  
  //Serial.begin(9600);
}

void loop()
{
  // refresh screen every 5 seconds
  refreshScreen();

  boolean resetOnly = false;
  if (_doNothing) // because service is required only service light will blink
  {
    _serviceLight = stateMachineHelper.blinkLigth(_serviceLight, ProgramConstants::_serviceLight);
    resetOnly = true;
  }
  else if (_preparedToSwitchOff)
  {
    // nothing to do
    // or 
    // user cannot press anything - set infinite loop
    while (true) 
    {  
      if (_serviceRequired)
      {
        _serviceLight = stateMachineHelper.blinkLigth(_serviceLight, ProgramConstants::_serviceLight);
      }
    }
  }
  else
  {
    // check operation time
    helper.checkOperationTime();
    
    if (_serviceRequired)
    {
      _serviceLight = stateMachineHelper.blinkLigth(_serviceLight, ProgramConstants::_serviceLight);
    }
    
    int service = helper.needService();
    if (service >= 300)
    {
      // machine needs a service, nothing can be selected any more
      _doNothing = true;
      // stop everything
      stopEverything();
      
      _aFirstLine = "Service needed!";
      printToFirstLine(_aFirstLine);
	  _aSecondLine = "";
      printToSecondLine(_aSecondLine);
    }
    else 
    {
      if (service >= 290 && !_serviceRequired)
      {	
        _serviceRequired = true;
      }
      
      if (!_programRunning)
      {
        // indicates that program is running
        _programRunning = true;
        
        // set program state / when program is started then 1. program is selected 
        _programStateInternal = 1;
		_selected = _programStateInternal;
        stateMachineHelper.setProgramState(_programStateInternal);
        StateMachineHelper::StateMachineResult result = stateMachineHelper.runCycle(1);
        update(result);
      }
      else
      {
        // check running program status
        if (!stateMachineHelper._waitForManuallTrigger)
        {
          StateMachineHelper::StateMachineResult result = stateMachineHelper.processCycle();
          update(result);
        }
        
        if (!_pumpRunning && !_preparedToSwitchOff)
        {
          // turn pump on after 3 s
          if ((now() - _startTime) > 3)
          {
            digitalWrite(ProgramConstants::_pumpLight, HIGH);
            _pumpRunning = true;
          }
        }
      }
    }
  }
  
  if (_lastPressed > 0 && ((now() - _lastPressed) > 5))
  {
    resetToLastValues();
  }
  
  checkButtonSelection(resetOnly);
}

void update(StateMachineHelper::StateMachineResult result)
{
  if (_aFirstLine != result.firstLineResult)
  {
    printToFirstLine(result.firstLineResult);
    _aFirstLine = result.firstLineResult;
  }
  
  if (_aSecondLine != result.secondLineResult)
  {
    printToSecondLine(result.secondLineResult);
    _aSecondLine = result.secondLineResult;
  }
  
  if (result.stopEverything)
  {
    stopEverything();
    _programStateInternal = 0;
    stateMachineHelper.setProgramState(_programStateInternal);
  }
}

void resetToLastValues()
{
  printToFirstLine(_aFirstLine);
  printToSecondLine(_aSecondLine);
  
  _lastPressed = 0;
  int state = stateMachineHelper.getProgramState();
  if (_programStateInternal != state)
  {
    stateMachineHelper.switchAllProgramLights(LOW);
    _programStateInternal = state;
    
    if (_programStateInternal == 1)
    {
      digitalWrite(ProgramConstants::_program1Light, HIGH);
    }
    else if (_programStateInternal == 2)
    {
      digitalWrite(ProgramConstants::_program2Light, HIGH);
    }
    else if (_programStateInternal == 3)
    {
      digitalWrite(ProgramConstants::_program3Light, HIGH);
    }
  }
}

void refreshScreen()
{
  if (!_lastTextPrinted) {
    printToFirstLine(_aFirstLine);
    printToSecondLine(_aSecondLine);
  }

  if (_lastDisplayRefreshTime == -1)
  {
    _lastDisplayRefreshTime = now();
    return;  
  }
  
  if (_lastPressed == 0 && (_lastDisplayRefreshTime + 5) < now())
  {
    _lastDisplayRefreshTime = now();
    //printToFirstLine("");
    //printToSecondLine("");
    
    printToFirstLine(_aFirstLine);
    printToSecondLine(_aSecondLine);
  }
}

void stopEverything()
{
  stopCycle(false);
  stopPump();
  
  _programRunning = false;
  stateMachineHelper._allowNewProgramSelection = false;
  _preparedToSwitchOff = true;
  
  digitalWrite(ProgramConstants::_flushingLight, LOW);
  digitalWrite(ProgramConstants::_UVLight, LOW);
}

void stopPump()
{
  _pumpRunning = false;
  digitalWrite(ProgramConstants::_pumpLight, LOW);
}

void stopCycle(boolean updateState)
{
  StateMachineHelper::StateMachineResult result = stateMachineHelper.stopCycle();
  if (updateState)
  {
    update(result);
  }
}



/**
*
* Program selection
*
*/
boolean _pressed = false;
void checkButtonSelection(boolean resetOnly)
{
  int pressedButton = getPressedButton();
  
  switch (pressedButton)
  {
    case btnRIGHT: // WASH
    {
      delay(300);
      if (resetOnly)
      {
        return;
      }
      
      if (!stateMachineHelper._allowNewProgramSelection)
      {
        return;
      }

      if (!_pressed)
      {
        _pressed = true;
        stateMachineHelper.activateWash();
        StateMachineHelper::StateMachineResult result = stateMachineHelper.runCycle(1);
        update(result);
      }
      
      break;
    }
    case btnLEFT:
    {
      // program selection
      delay(300);
      if (resetOnly)
      {
        return;
      }
      
      if (!stateMachineHelper._allowNewProgramSelection)
      {
        return;
      }
      
      if (!_pressed)
      {
        _pressed = true;
        _lastPressed = now();
        _programStateInternal++;
        if (_programStateInternal > 4)
        {
          _programStateInternal = 1;
        }
      
        switchProgram();
      }
      
      break;
    }
    case btnSELECT:
    {
      delay(300);
      if (resetOnly)
      {
        resetServiceCounter(btnSELECT);
        return;
      }
      
      if (!stateMachineHelper._allowNewProgramSelection)
      {
        return;
      }
      
      if (!_pressed)
      {
        resetServiceCounter(btnSELECT);
        _pressed = true;
        
        // confirmation program
        if ((_programStateInternal > 0 && _programStateInternal < 4) || // program 1, 2, 3
          (_programStateInternal == 4 && !stateMachineHelper._waitForManuallTrigger)) // desinfection
        {
          if (_selected == _programStateInternal) 
          {
            return; // program is running nothing to do
          }
                 
          // reset to init positions
          stopCycle(true);
          stateMachineHelper.setProgramState(_programStateInternal);
          StateMachineHelper::StateMachineResult result = stateMachineHelper.runCycle(_programStateInternal == 4 ? 6 : 1);
          update(result);
          
          _selected = _programStateInternal;
        }
        else if (_programStateInternal == 4 && stateMachineHelper._waitForManuallTrigger) // end. desinfection
        {
          delay(400);
          
          stateMachineHelper._waitForManuallTrigger = false;
          stateMachineHelper._allowNewProgramSelection = false;
          stateMachineHelper.setRunningCycleSequenceId(1);
          _programStateInternal = 0;
          stateMachineHelper.setProgramState(_programStateInternal);
          
          resetToLastValues();
          _aSecondLine = "closing";
		  printToSecondLine(_aSecondLine);
        }
        
        _lastPressed = 0;
      }
      
      break;
    }
    case btnUP:
    {
      delay(300);
      if (!resetOnly && !stateMachineHelper._allowNewProgramSelection)
      {
        return;
      }
      
      if (!_pressed)
      {
        _pressed = true;
        _lastPressed = now();
        showOperationDetails();
        resetServiceCounter(btnUP);
      }  
  
      break;
    }
    case btnDOWN:
    { 
      break;
    }
    case btnNONE:
    {
      _pressed = false;
      break;
    }
  }
}

void resetServiceCounter(int key)
{
  if (helper.resetServiceCounter(key))
  {
    printToSecondLine("Counter is reset");
    delay(1000);
    
    _serviceRequired = false;
    if (_doNothing)
    {
      _doNothing = false;
      _preparedToSwitchOff = false;
    }
  }
}

int getPressedButton()
{
  int analogInput = analogRead(0);      // read the value from the sensor

  if (analogInput > 1000) 
  {	
    return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  }
  
  if (analogInput < 50)   //50
  {
    return btnRIGHT;  
  }
  
  if (analogInput < 250)  //250 // 200
  {
    return btnUP;
  }
  
  if (analogInput < 450)  //450 // 300
  {
    //return btnDOWN;
    return btnSELECT;
  }
  
  if (analogInput < 720)  //720  // 500
  {
    return btnLEFT; 
  }
  
  /*if (analogInput < 850)  //850 //700
  {
    return btnSELECT;  
  }*/
  
  return btnNONE;  // when all others fail, return this...
}

void switchProgram()
{
  switch (_programStateInternal)
  {
    case 1:
    {
      digitalWrite(ProgramConstants::_program1Light, HIGH);
      digitalWrite(ProgramConstants::_desinfectionLight, LOW);
      printToFirstLine("Select program 1");
    }
    break;
    case 2:
    {
      digitalWrite(ProgramConstants::_program2Light, HIGH);
      digitalWrite(ProgramConstants::_program1Light, LOW);
      printToFirstLine("Select program 2");
    }
    break;
    case 3:
    {
      digitalWrite(ProgramConstants::_program3Light, HIGH);
      digitalWrite(ProgramConstants::_program2Light, LOW);
      printToFirstLine("Select program 3");
    }
    break;
    case 4:
    {
      digitalWrite(ProgramConstants::_desinfectionLight, HIGH);
      digitalWrite(ProgramConstants::_program3Light, LOW);
      printToFirstLine("Select desinf.");
    }
    break;
  }
}

void showOperationDetails()
{
  int operation = helper.getOperationTime();                
  int fromService = helper.getFromServiceTime();
  int quarters = helper.getQuarters();
              
  long time = now();
  displayOperationDetails(operation, (operation - fromService), (time - _startTime)/60, quarters);
}

/**
*
*	Working with LCD screen (I don't know how to use LiquidCrystal.h library in another library)
*
**/
void printToFirstLine(char* text)
{
  printToLCD(text, 0, 0, true);
}

void printToSecondLine(char* text)
{
  printToLCD(text, 0, 1, true);
}

void printToLCD(char* text, int column, int line, boolean clearLine)
{
  // 3v(what we want)/2.5 (reference) x1024/2=614
  //we read VSS/2 on port 8, we want more than 2.5v to write on the lcd
  if (analogRead(8) > 613) {
    _lastTextPrinted = true;
  }
  else {
    // do not print text to lcs, if voltage is to low
    _lastTextPrinted = false;
    delay(50);
    return;
  }

  //lcd.clear();
  //delay(1000);

  if (clearLine)
  {
    lcd.setCursor(column, line);
    lcd.print("                ");
  }
  
  lcd.setCursor(column, line);
  lcd.print(text);
}

void displayOperationDetails(int operationTime, int fromServiceTime, long runnigTime, int quarters)
{
  printToFirstLine("");
  lcd.setCursor(0, 0);
  lcd.print("O: ");
  lcd.setCursor(2, 0);
  lcd.print(operationTime);
  lcd.setCursor(11, 0);
  lcd.print("S:");
  lcd.setCursor(13, 0);
  lcd.print(fromServiceTime);
  printToSecondLine("");
}

