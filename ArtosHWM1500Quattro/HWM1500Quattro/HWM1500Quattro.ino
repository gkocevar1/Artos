#include <LiquidCrystal.h>
#include "Constants.h"
#include "HWM1500Quattro_debug.h" 

#define btnRIGHT  0 // flushing
#define btnUP     1 // service info
#define btnDOWN   2
#define btnLEFT   3 // program selection
#define btnSELECT 4 // confirmation button
#define btnNONE   5

// init LCD panel
LiquidCrystal _lcd(8, 9, 4, 5, 6, 7);

/**
 * Setup program parameters
 */
void setup()
{
  // init LCD screen
  _lcd.begin(16, 2);
  _lcd.setCursor(0,0);

  // init serial port
  Serial.begin(9600);
}

/**
 * Main loop
 */
void loop()
{
  
  

  
  delay(1000);
}

// ------------------
// Helper methods to work with LCD display

void printToBothLines(char *textLine1, char *textLine2)
{
  printToFirstLine(textLine1);
  printToSecondLine(textLine2);
}

/**
 * print text for first line
 */
void printToFirstLine(char *text)
{
  printToLCD(text, 0, 0, true);
}

/**
 * print text to second line
 */
void printToSecondLine(char *text)
{
  printToLCD(text, 0, 1, true);
}

/**
 * print text to LCD
 */
void printToLCD(char* text, int column, int line, boolean clearLine)
{
  if (clearLine)
  {
    printToLCDLine("          ", column, line);
  }
  
  printToLCDLine(text, column, line);
}

/**
 * print text to LCD line
 */
void printToLCDLine(char* text, int column, int line)
{
  _lcd.setCursor(column, line);
  _lcd.print(text);
}
// ------------------

