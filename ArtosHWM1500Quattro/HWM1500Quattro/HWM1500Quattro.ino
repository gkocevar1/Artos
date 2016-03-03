/**
   HWM1500Quattro workflow

   When machine is turned on wash program is started immediately (sequence 1A to 1C). After 3 seconds pump is turned on.

   Program 1 is started automatically when wash program is completed.
   During the first 2A phase, user can select different programs (program 1, program 2, program 3 or desinfection). Otherwise selection is disabled.

   Wash program is active during all 2A phases. When user press on wash button, phases 1A to 1C are executed.
   Once wash program is completed, 2A phase is continue for selected program.


   Desinfection program can be selected only during first 2A phase. If desinfection is selected, valves must be moved to proper positions.
   Nothing else can be selected during that phase. Only enter button must be active. By pressing the end button all vales will be moved to close position.
   Desinfection light must blink during period. TODO: find out how many seconds.

   UV light is turned on when pump is running and valve 9 is open.
*/

#include <Vector2.h>
#include <LiquidCrystal.h>
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

LiquidCrystal _lcd(8, 9, 4, 5, 6, 7);
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
boolean _serviceNeeded = false;






/*
  Setup program parameters
*/
void setup() {
  // init LCD screen
  _lcd.begin(16, 2);
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
  delay(100);
  // turn on power light
  digitalWrite(Constants::PowerOnLight, HIGH);
}

/*
  Main loop
*/
void loop() {

  // TODO: ce takoj po filtraciji prizges wash, pocakas do konca in pritisnes left(enter) se zgodi nekaj cudnega
  // TODO: mogoce bi lahko omogocili, da lahko uporabnik med washom (samo prvic) izbere kateri program si zeli, vendar mora pocakati do konca wash periode

  // TODO:DONE select between programs(1,2,3) - this can be selected only during first 2A phase (each time)
  // TODO:DONE confirm selected program
  // TODO:DONE select wash (special case: this can be selected always during phase 2A)
  // TODO: select desinfection - this can be selected only during first 2A phase (each time) - confirmation needed to close valves and blink with desinfection light
  // TODO:DONE set wash ligth to on during wash cycle (also when machine is turned on)
  // TODO:DONE reset display to previous values after 5 seconds
  // TODO:DONE display status,
  // TODO:DONE update operation time
  // TODO:DONE if service needed blink the light (or display on screen),
  // TODO: stop machine if service time exceeded,

  if (_ms.checkOperationTime())
  {
    if (!_serviceNeeded)
    {
      // only reset is allowed
      printToBothLines("Service needed!", "");
      _serviceNeeded = true;
    }
  }
  else
  {
    if (!_programRunning)
    {
      _programRunning = true;
      // when pump is turned on wash program must be executed first
      // continue with program 1 when wash is finished
      _sm.runProgram(Constants::Program::ProgramWash, true);
    }
    else
    {
      _sm.checkProgress();
    }

    // update display with text
    updateDisplay();
  }

  // check when buttons were last pressed (set old display values after 5 seconds)
  checkLastPressedButton();
  // check if users pressed a button
  checkUserSelection();
}

// -------------------------
// AUTOMATIC SELECTIONS



// --------------------------
// USER SELECTION

boolean _pressed = false;
void checkUserSelection()
{
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

          DMSG("WASH button is pressed");
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

          DMSG("SELECT button is pressed");
          _pressed = true;

          if (_sm.isProgramChangeAllowed())
          {
            DMSG("CALL SWITCH PROGRAM");
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
          DMSG("ENTER button is pressed");
          _pressed = true;

          if (_programToSelect != -1 && _sm.isProgramChangeAllowed())
          {
            // clear display
            printToBothLines("", "");

            Constants::Program program = static_cast<Constants::Program>(_programToSelect);
            _sm.runProgram(program, false);

            _programToSelect = -1;
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
          DMSG("STATUS button is pressed");
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

  if (analogInput < 50)
  {
    return btnRIGHT;
  }

  if (analogInput < 200)
  {
    return btnUP;
  }

  if (analogInput < 300)
  {
    return btnDOWN;
  }

  if (analogInput < 500)
  {
    return btnLEFT;
  }

  if (analogInput < 700)
  {
    return btnSELECT;
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

  DMSG1("Switch to next program: "); DMSG(Constants::ProgramNames[_programToSelect]);

  setProgramLights(static_cast<Constants::Program>(_programToSelect));

  printToFirstLine("Select");
  printToSecondLine(Constants::ProgramNames[_programToSelect]);
}

/**
   check when buttons were last pressed (set old display values after 5 seconds)
*/
void checkLastPressedButton()
{
  //DMSG("---------checkLastPressedButton---------------");
  //DMSG1(_lastPressed);DMSG1(" ");DMSG1(now());
  if (_lastPressed > -1 && ((now() - _lastPressed) > 5))
  {
    //DMSG("---------IN---------------");
    _aFirstLine = "";
    _aSecondLine = "";

    _lastPressed = -1;
    _programToSelect = -1;

    // turn of all lights (program1, program2, program3, desinfection) except running one
    setProgramLights(_sm.runningProgram);
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
  _lcd.setCursor(0, 1);
  _lcd.print("Q: ");
  _lcd.setCursor(2, 1);
  _lcd.print(_ms.machineStatus.quarters);
}

/**
   reset operation time
*/
void resetOperationTime(int key)
{
  DMSG1("reset operation time - key: "); DMSG(key);

  if 
  printToFirstLine("Counter is reset");
  printToSecondLine("");
}

// --------------------------
// LIQUID CRYSTAL

/**
   update text on display.
   Last text will be saved in memory and new text will be compared with last text and updated only if different.
*/
void updateDisplay()
{
  char* program = Constants::ProgramNames[_sm.runningProgram];
  char *phase = Constants::PhaseNames[_sm.runningPhase];

  printToBothLines(program, phase);
}

/*
  print to both lines
*/
void printToBothLines(char *textLine1, char *textLine2)
{
  if (textLine1 != _aFirstLine)
  {
    DMSG("Refresh line 1");
    _aFirstLine = textLine1;
    //if (_lastPressed == -1)
    //{
    printToFirstLine(textLine1);
    //}
  }

  if (textLine2 != _aSecondLine)
  {
    DMSG("Refresh line 2");
    _aSecondLine = textLine2;
    //if (_lastPressed == -1)
    //{
    printToSecondLine(textLine2);
    //}
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
void printToLCD(char* text, int column, int line, boolean clearLine)
{
  if (clearLine)
  {
    printToLCDLine("                ", column, line);
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

