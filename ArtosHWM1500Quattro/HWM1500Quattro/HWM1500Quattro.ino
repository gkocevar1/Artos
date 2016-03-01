#include <Vector.h>
#include <LiquidCrystal.h>
#include "HWM1500Quattro_debug.h"
#include "Constants.h"
#include "StateMachine.h"

LiquidCrystal _lcd(8, 9, 4, 5, 6, 7);
StateMachine _sm;



char* _aFirstLine;
char* _aSecondLine;

boolean _programRunning = false;





/*
Setup program parameters
*/
void setup() {
	// init LCD screen
	_lcd.begin(16, 2);
	_lcd.setCursor(0, 0);

	// init serial port
	Serial.begin(9600);
}

/*
Main loop
*/
void loop() {
  
	if (!_programRunning)
	{
		_programRunning = true;

		_sm.runProgram(Constants::Program::Program2);
	}
	else
	{
		// check state
		DMSG1(millis() / 1000);DMSG1(" ");
		
		_sm.checkProgress();
	}

  updateDisplay();

	delay(1000);
}

void updateDisplay()
{
  char* program;
  char *phase = Constants::PhaseNames[_sm.runningPhase];
  
  switch(_sm.runningProgram)
  {
    case Constants::Program::Program1:
    {
      program = "Program: 1";
      break;
    }
    case Constants::Program::Program2:
    {
      program = "Program: 2";
      break;
    }
    case Constants::Program::Program3:
    {
      program = "Program: 3";
      break;
    }
  }
  
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
	  printToFirstLine(textLine1);
  }

  if (textLine2 != _aSecondLine)
  {
    DMSG("Refresh line 2");
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

