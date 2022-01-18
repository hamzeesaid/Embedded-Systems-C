/********************************************************************/
// HC12 Program:  ICA8- Introduction to the LCD Display
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza
/* Details:       This Program performs some basic LCD based operations.*/

/********************************************************************/

/********************************************************************/
// Library includes
/********************************************************************/


#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "hal.h"
#include "SwLED_Lib.h"
#include "SevSeg_Lib.h"
#include "SCI0_Lib.h"
#include "LCD_Lib.h"


/********************************************************************/
// Local Prototypes
/********************************************************************/
void Timer_msTick(void);
/********************************************************************/
// Global Variables
/********************************************************************/
char helloString[14] = "Hello World!"; 
int msCounter, secCounter;
char update = 0;
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
  HAL_SetClk24();
  EnableInterrupts;

  /********************************************************************/
  // initializations
  /********************************************************************/
  SwLED_Init();
  SevSeg_Init();
  LCD_Init();
  LCD_Clear();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  
  // display initials with cursor blink
  LCD_Addr(8);
  LCD_String("HS");
  LCD_Ctrl(0b00001111);

  for (;;)
  {
    
    // down switch ,show "Hello World!" on the bottom line, 
    //  with no cursor visible 
    if (PT1AD1 & SW_DOWN)
    {
      LCD_Addr(88);
      LCD_String(helloString);
      LCD_Ctrl(0b00001100);
    }
    
    // when top switch is pressed, 
    // clear the screen and show the cursor with no blinking,
    // at very fist position on the top line
    if (PT1AD1 & SW_UP)
    {
      LCD_Clear();
      LCD_Addr(LCD_ROW0);
      LCD_Ctrl(0b00001110);
    }
    
    // when middle switch is pressed,
    // go back to the initials part
    if (PT1AD1 & SW_MID)
    {
      LCD_Clear();
  // display initials with cursor blink
  LCD_Addr(0x8);
  LCD_Char('H');
  LCD_Addr(9);
  LCD_Char('S');

  LCD_Ctrl(0b00001111);
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
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/