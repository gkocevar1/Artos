/**
   HWM1500Quattro workflow

   When machine is turned on wash program is started immediately (sequence 1A to 1C). After 3 seconds pump is turned on.

   Program 1 is started automatically when wash program is completed.
   During the filtration phase (2A), user can select different programs (program 1, program 2, program 3 or desinfection).

   Wash program is active during all 2A phases. When user press on wash button, phases 1A to 1C are executed.
   Once wash program is completed, 2A phase is continue for selected program.

   Desinfection program can be selected only during first 2A phase. If desinfection is selected, valves must be moved to proper positions. Duration of desinfaction
   phase is unlimited. Nothing else can be selected during that phase. Only enter button must be active.
   By pressing the enter button all valves will be moved to close position. Desinfection light must blink during close period.

   UV light is turned on when pump is running and valve 9 is open.

   To reset service timer press within 5 second status, enter, status, enter button.
   TODO: Possible update: press status button for 5 seconds and then within 5 second press status, enter, status, enter button.
   TODO: Possible update: program selection during wash start-up period
*/

#include <Vector.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HWM1500Quattro_debug.h"
#include "Constants.h"
#include "StateMachine.h"
#include "MachineStatus.h"

// button PIN constants
#define btnRIGHT  0 // flushing
#define btnUP     1 // service info
#define btnDOWN   2
#define btnLEFT   3 // program selection
#define btnSELECT 4 // confirmation button
#define btnNONE   5

// 0x27 lcd address
LiquidCrystal_I2C _lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

StateMachine _sm;
MachineStatus _ms;

char* _aFirstLine;
char* _aSecondLine;
boolean _programRunning = false;
unsigned long _start = now();

// program to select manually (only for internal state)
int _programToSelect = -1;
// last pressed button time (if status / switch between programs button is not pressed for 5 seconds then return display to previous text)
long _lastPressed = -1;
// if service is needed, program selection is not allowed except reset counter
// when this flag is set to true, nothing is working
boolean _serviceNeeded = false;
// last blink check time (for desinfection light - when close program is in progress)
long _lastBlinkCheckTime = -1;

/*
  Setup program parameters
*/
void setup() {
  // init LCD screen
  _lcd.begin (16,2);
  _lcd.setBacklight(HIGH);
  _lcd.setCursor(0, 0);

  // init serial port
  Serial.begin(9600);

  // init all pins
  // valves
  pinMode(Constants::Valve1P, OUTPUT);
  pinMode(Constants::Valve1M, OUTPUT);
  pinMode(Constants::Valve2P, OUTPUT);
  pinMode(Constants::Valve2M, OUTPUT);
  pinMode(Constants::Valve3P, OUTPUT);
  pinMode(Constants::Valve3M, OUTPUT);
  pinMode(Constants::Valve4P, OUTPUT);
  pinMode(Constants::Valve4M, OUTPUT);
  pinMode(Constants::Valve5P, OUTPUT);
  pinMode(Constants::Valve5M, OUTPUT);
  pinMode(Constants::Valve6P, OUTPUT);
  pinMode(Constants::Valve6M, OUTPUT);
  pinMode(Constants::Valve7P, OUTPUT);
  pinMode(Constants::Valve7M, OUTPUT);
  pinMode(Constants::Valve8P, OUTPUT);
  pinMode(Constants::Valve8M, OUTPUT);
  pinMode(Constants::Valve9P, OUTPUT);
  pinMode(Constants::Valve9M, OUTPUT);
  pinMode(Constants::Valve10P, OUTPUT);
  pinMode(Constants::Valve10M, OUTPUT);
  pinMode(Constants::Valve11P, OUTPUT);
  pinMode(Constants::Valve11M, OUTPUT);
  pinMode(Constants::Valve12P, OUTPUT);
  pinMode(Constants::Valve12M, OUTPUT);
  pinMode(Constants::Valve13P, OUTPUT);
  pinMode(Constants::Valve13M, OUTPUT);
  pinMode(Constants::Valve14P, OUTPUT);
  pinMode(Constants::Valve14M, OUTPUT);
  // lights
  pinMode(Constants::Program1Light, OUTPUT);
  pinMode(Constants::Program2Light, OUTPUT);
  pinMode(Constants::Program3Light, OUTPUT);
  pinMode(Constants::DesinfectionLight, OUTPUT);
  pinMode(Constants::WashLight, OUTPUT);
  pinMode(Constants::PumpLight, OUTPUT);
  pinMode(Constants::UVLight, OUTPUT);
  pinMode(Constants::PowerOnLight, OUTPUT);
}

/*
  Main loop
*/
void loop() {

  if (_ms.checkOperationTime())
  {
    if (!_serviceNeeded)
    {
      // only reset is allowed
      printToBothLines("Service needed!", "");
      _serviceNeeded = true;
      // put machine in idle state
      idle();
    }
  }
  else
  {
    if (!_programRunning)
    {
      delay(100);
      // turn on power light
      digitalWrite(Constants::PowerOnLight, HIGH);

      _programRunning = true;
      // when pump is turned on wash program must be executed first
      // continue with program 1 when wash is finished
      _sm.runProgram(Constants::Program::ProgramWash, true);
    }
    else
    {
      if (_sm.runningProgram != Constants::Program::ProgramNone)
      {
        _sm.checkProgress();

        if (_sm.runningProgram == Constants::Program::ProgramClose)
        {
          // blink with desinfection light, when close program is running after desinfection program / phase
          _lastBlinkCheckTime = _ms.blinkLigth(_lastBlinkCheckTime, Constants::DesinfectionLight);
        }
        else if (_sm.runningProgram == Constants::Program::ProgramNone)
        {
          idle();
          printToSecondLine("ends. Turn off.");
        }
      }
    }

    // update display with text
    updateDisplay();
  }

  // check when buttons were last pressed (set old display values after 5 seconds)
  checkLastPressedButton();
  // check if users pressed a button
  checkUserSelection();
}

// --------------------------
// AUTOMATIC SET

/**
   put machine in idle state
   1. if service is needed and nothing can be run
   2. after close program (desinfection)
*/
void idle()
{
  // turn off pump, all program lights, force stop valves
  digitalWrite(Constants::Program1Light, LOW);
  digitalWrite(Constants::Program2Light, LOW);
  digitalWrite(Constants::Program3Light, LOW);
  digitalWrite(Constants::DesinfectionLight, LOW);
  digitalWrite(Constants::WashLight, LOW);
  digitalWrite(Constants::PumpLight, LOW);
  digitalWrite(Constants::UVLight, LOW);
  _sm.deactivateValves();
  //_programRunning = false;
}

// --------------------------
// USER SELECTION

boolean _pressed = false;
void checkUserSelection()
{
  if (_sm.runningProgram == Constants::Program::ProgramNone)
  {
    return;
  }

  int pressedButton = getPressedButton();

  switch (pressedButton)
  {
    case btnRIGHT:
      {
        // trigger wash program (WASH button on device)
        delay(300);
        if (!_pressed)
        {
          if (_serviceNeeded)
          {
            return;
          }

          _pressed = true;
          if (_sm.isWashAllowed())
          {
            _sm.runProgram(Constants::Program::ProgramWash, false);
          }
        }

        break;
      }
    case btnLEFT:
      {
        // choose between options (SELECT button on device)
        delay(300);
        if (!_pressed)
        {
          if (_serviceNeeded)
          {
            return;
          }

          _pressed = true;

          if (_sm.isProgramChangeAllowed())
          {
            switchToNextProgram();

            _lastPressed = now();
          }
        }

        break;
      }
    case btnSELECT:
      {
        // select program (ENTER button on device)
        delay(300);
        if (!_pressed)
        {
          _pressed = true;

          if (_sm.runningPhase == Constants::Phase::Desinfection)
          {
            _sm.runProgram(Constants::Program::ProgramClose, false);
          }
          else if (_programToSelect != -1 && _sm.isProgramChangeAllowed())
          {
            // clear display
            printToBothLines("", "");

            Constants::Program program = static_cast<Constants::Program>(_programToSelect);
            _sm.runProgram(program, false);

            _programToSelect = -1;
            _lastPressed = -1;
          }

          resetOperationTime(btnSELECT);
        }

        break;
      }
    case btnUP:
      {
        // status of device (STATUS button on device)
        delay(300);
        if (!_pressed)
        {
          _pressed = true;
          displayStatus();

          resetOperationTime(btnUP);

          _lastPressed = now();
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

/**
   get pressed button
*/
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
    return btnSELECT;
  }
  
  if (analogInput < 720)  //720  // 500
  {
    return btnLEFT; 
  }
  
  return btnNONE;  // when all others fail, return this...
}

/**
   calculate which program is next in sequence:
      Program1, Program2, Program3, Desinfection and repeat

   then switch program (switch light and prepare program for selection)
   if user doesn't confirm selection then values are reset after 5 seconds (display, selection ...)
*/
void switchToNextProgram()
{
  if (_programToSelect == -1)
  {
    // get current running program
    _programToSelect = _sm.runningProgram;
  }

  // if current running program is equals to desinfection/wash/close, set programToSelect parameter to Program1 = 0
  if (_programToSelect >= 3) //Constants::Program::ProgramDesinfection)
  {
    _programToSelect = 0; //Constants::Program::Program1;
  }
  else
  {
    // update program to select (max. to desinfection)
    ++_programToSelect;
  }

  setProgramLights(static_cast<Constants::Program>(_programToSelect));

  printToFirstLine("Select");
  printToSecondLine(Constants::ProgramNames[_programToSelect]);
}

/**
   check when buttons were last pressed (set old display values after 5 seconds)
*/
void checkLastPressedButton()
{
  if (_lastPressed == -1 || ((now() - _lastPressed) < 6))
  {
    return;
  }

  _aFirstLine = "";
  _aSecondLine = "";

  _lastPressed = -1;
  _programToSelect = -1;

  // turn of all lights (program1, program2, program3, desinfection) except running one
  setProgramLights(_sm.runningProgram);
  // if wash program is running then turn on light for program1/program2/program3
  if (_sm.runningProgram == Constants::Program::ProgramWash)
  {
    setProgramLights(_sm.programToRunAfterWash);
  }
}

void setProgramLights(Constants::Program program)
{
  digitalWrite(Constants::Program1Light, (program == Constants::Program::Program1) ? HIGH : LOW);
  digitalWrite(Constants::Program2Light, (program == Constants::Program::Program2) ? HIGH : LOW);
  digitalWrite(Constants::Program3Light, (program == Constants::Program::Program3) ? HIGH : LOW);
  digitalWrite(Constants::DesinfectionLight, (program == Constants::Program::ProgramDesinfection) ? HIGH : LOW);
}

void displayStatus()
{
  printToFirstLine("");
  _lcd.setCursor(0, 0);
  _lcd.print("O: ");
  _lcd.setCursor(2, 0);
  _lcd.print(_ms.machineStatus.operationTime);
  _lcd.setCursor(11, 0);
  _lcd.print("S:");
  _lcd.setCursor(13, 0);
  _lcd.print((_ms.machineStatus.operationTime - _ms.machineStatus.serviceTime));

  printToSecondLine("");
}

/**
   reset operation time
*/
void resetOperationTime(int key)
{
  if (_ms.resetOperationTime(key))
  {
    printToFirstLine("Counter is reset");
    printToSecondLine("");
    _lastPressed = now();

    if (_serviceNeeded)
    {
      printToSecondLine("Starting wash");
      _serviceNeeded = false;
      _lastPressed = -1;
      _programRunning = false;

      delay(5000);
    }
  }
}

// --------------------------
// LIQUID CRYSTAL

/**
   update text on display.
   Last text will be saved in memory and new text will be compared with last text and updated only if different.
*/
void updateDisplay()
{
  if (_sm.runningProgram == Constants::Program::ProgramNone)
  {
    return;
  }

  if (_lastPressed != -1)
  {
    return;
  }

  char* program = Constants::ProgramNames[_sm.runningProgram];
  char *phase = Constants::PhaseNames[_sm.runningPhase];

  printToBothLines(program, phase);
}

void printToBothLines(char *textLine1, char *textLine2)
{
  if (textLine1 != _aFirstLine)
  {
    _aFirstLine = textLine1;
    printToFirstLine(textLine1);
  }

  if (textLine2 != _aSecondLine)
  {
    _aSecondLine = textLine2;
    printToSecondLine(textLine2);
  }
}

/*
  print text for first line
*/
void printToFirstLine(char *text)
{
  printToLCD(text, 0, 0, true);
}

/*
  print text to second line
*/
void printToSecondLine(char *text)
{
  printToLCD(text, 0, 1, true);
}

/*
  print text to LCD
*/
char* emptyLine = "                ";
void printToLCD(char* text, int column, int line, boolean clearLine)
{
  if (clearLine)
  {
    printToLCDLine(emptyLine, column, line);
  }

  printToLCDLine(text, column, line);
}

/*
  print text to LCD line
*/
void printToLCDLine(char* text, int column, int line)
{
  _lcd.setCursor(column, line);
  _lcd.print(text);
}

