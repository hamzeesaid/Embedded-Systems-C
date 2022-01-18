// HC12 Program:	YourProg - MiniExplanation
// Processor:	MC9S12XDP512
// Xtal Speed:	16 MHz
// Author:		Hamza Said
// Date:			octuber 4
//
// Details: This program counts down from 7 to 0 on the seven segment display
//          and it shows no decoded customized characters. All of this is done
//          using the up and down button.
//																		*
//************************************************************************/

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
#include "SevSeg_Lib.h"

/********************************************************************/

// Local Prototypes
/********************************************************************/
void Delay_500us(void);
/********************************************************************/
// Global Variables
int i;
int index;
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
  SevSeg_Init();
  SwLED_Init();
  /********************************************************************/
  // main program loop
  /********************************************************************/

  for (;;)
  {

    //if down switch is pressed, clear the seven seg. and write the specified characters
    if (PT1AD1 & SW_DOWN)
    {

      SevSeg_ClearAll();
      SevSeg_Top4(0x8AE3);
      SevSeg_CharD(4, 2);   //write 2 on address 4 with decimal point
      SevSeg_Char(5, 5);    //write 5 on address 5 no decimal
      SevSeg_Cust(6, 0xE6); //turn on the top 4 segments of address 6 with no decimal
      SevSeg_Char(7, 0xC);  // display c on the lasts egment
    }

    //if up switch is pressed, write the numbers 7 to 1 on the top and bottom
    if (PT1AD1 & SW_UP)
    {
      SevSeg_ClearAll();
      for (index = 0; index < 8; index++)
      {
        SevSeg_Char(index, 7 - index);
        Delay_500us();
      }
    }
  }
}

/********************************************************************/
// Functions
/********************************************************************/
void Delay_500us()
{
  for (i = 0; i < 50; i++)
  {
    Delay10ms_asm();
  }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/