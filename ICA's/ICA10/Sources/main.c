
/********************************************************************/
// HC12 Program:  ICA10- Time-Lapse LCD Display
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza
/* Details:       This program displays a message on any button click using delay of given milliseconds between each character*/

// Date:          9/11/2021


/********************************************************************/

/********************************************************************/
// Library includes
/********************************************************************/


#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "hal.h"
#include "SwLED_Lib.h"
#include "SevSeg_Lib.h"
#include "LCD_Lib.h"
#include "stdio.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/
void Timer_msTick(void);
int msCounter, secCounter;
char update = 0;

// check button press
unsigned char CheckPresses(void);


//  write on a button press 
void selectAny(void);

/********************************************************************/
// Global Variables
/********************************************************************/
char buttonPrompt[25] = "Press any button:";

char secondLine[25] = "A long time ago in a";
char thirdLine[25] = "galaxy far, far away";

// state machine 
enum State
{
  Menu,
  anySwitch
};
enum State mainState = Menu;
/********************************************************************/
// Constants
/********************************************************************/

/********************************************************************/
// Main Entry
/********************************************************************/
void main(void)
{
  // main entry point
  _DISABLE_COP();
  HAL_Init();
  HAL_RtiInit(&Timer_msTick);  
  /********************************************************************/
  // initializations
  /********************************************************************/
  EnableInterrupts;
  
  LCD_Init();
  SwLED_Init();
  SevSeg_Init();

  /********************************************************************/
  // main program loop
  /********************************************************************/

  for (;;)
  {
    switch (mainState)
    {
    
    case Menu:
      
      // check for presses in main menu, exit if a press is detected
      if (CheckPresses())
      { 
        break;
      }

      // start a program with showing a given message on screen
      LCD_Init();
      LCD_Addr(0);
      LCD_String(buttonPrompt);

      break;
      
    case anySwitch:
      
      //  display message progressively
      if (PT1AD1)
      {
        selectAny();
      }
      break;

    default:
      break;
    }
  }
}

/********************************************************************/
// Functions
/********************************************************************/
void Timer_msTick()
{
  if(++msCounter > 999)
  {
    msCounter = 0;
    ++secCounter;
    update = 1;
  }
}
// function to write message on a button press 
void selectAny()
{

  LCD_Init();
  LCD_Pos(0, 0);
  LCD_StringDelay(secondLine);
  LCD_Pos(1, 0);
  LCD_StringDelay(thirdLine);
  LCD_Pos(3, 1);
  LCD_String(buttonPrompt);
  mainState = anySwitch;
}

// function to check button press
unsigned char CheckPresses()
{
  unsigned char pressDetected = 0;

  if (PT1AD1)
  {
    LCD_Clear();
    pressDetected = 1;
    selectAny();
  }

  return pressDetected;
}
