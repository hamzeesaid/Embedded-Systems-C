/********************************************************************/
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza Said
// Details:

// Date:          1/10/2021
// Revision History :
//  each revision will have a date + desc. of changes

/********************************************************************/
// Constant Defines
/********************************************************************/

/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
/********************************************************************/
// Library includes
/********************************************************************/
// your includes go here
#include "hal.h"
#include "SwLED_Lib.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/
void Delay_500us(void);
/********************************************************************/
// Global Variables
/********************************************************************/
char check;
int k;

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
  //EnableInterrupts;

  /********************************************************************/
  // initializations
  /********************************************************************/
  SwLED_Init();
  HAL_SetClk24();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    //check the status of upswitch , return true if its pressed .

    if (PT1AD1 & SW_UP)
    {
      check = 1;
    }

    //fortrue, toggle all LED's with a delay off 500ms
    if (check == 1)
    {
      Delay_500us();
      LED_Tog('A');
    }

    //If down switch is pressed, quit the special state and turn all LED's off
    if (PT1AD1 & SW_DOWN)
    {
      check = 0;
      if (check == 0)
      {
        LED_Off('A');
      }
    }
    //If Left switch is pressed, turn Red LED on
    if (PT1AD1 & SW_LEFT)
    {
      LED_On('R');
    }
    //If Right switch is pressed, turn Green LED on
    if (PT1AD1 & SW_RIGHT)
    {
      LED_On('G');
    }
    //If Mid switch is pressed, turn Yellow LED on
    if (PT1AD1 & SW_MID)
    {
      LED_On('Y');
    }
  }
}

/********************************************************************/
// Functions
/********************************************************************/
void Delay_500us()
{
  for (k = 0; k < 50; k++)
  {
    Delay10ms_asm();
  }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
