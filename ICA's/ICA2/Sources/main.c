/********************************************************************/
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza Said
// Details:       A more detailed explanation of the program is entered here

// Date:          Date Created
// Revision History :
//  each revision will have a date + desc. of changes

/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
/********************************************************************/
// Library includes
/********************************************************************/

// your includes go here
//#include "misc.h"
//#include "muxled.h"
//#include "sw_led.h"

//#include "SwLED_Lib.h"

#define SW_MID 0b00000001
#define SW_RIGHT 0b00000010
#define SW_DOWN 0b00000100
#define SW_LEFT 0b00001000 //switch4
#define SW_UP 0b00010000   //0x10

#define RED_LED 0b10000000
#define YELLOW_LED 0b01000000
#define GREEN_LED 0b00100000

/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
enum SwState
{
  Idle,
  Pressed,
  Held,
  Released,
};
enum SwState swMidState = Idle;
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
  PT1AD1 = 0b10000000; //start with green on and red LEDs off

  DDR1AD1 = 0b11100000; //LEDs as outputs, switches as inputs

  ATD1DIEN1 = 0b00011111; //enable the buttons and digital switch Input

  //DDRM |= DDRM_DDRM7_MASK; //Set PM7 to output

  PTM = 0; //initialaze the port M(in the diagram it is called PTM)

  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    if (PT1AD1 & SW_LEFT) //Turn Red Led on when left switch is pressed
    {
      //reset the Green LED

      PT1AD1 |= RED_LED;
      PT1AD1 &= ~GREEN_LED;

      DDRM &= ~DDRP_DDRP7_MASK;
      PTM &= ~DDRP_DDRP7_MASK;
    }
   
    else 
  {if (PT1AD1 & SW_MID)
    { //Switch active - HIGH level
      if (swMidState == Idle)
      { //Switch was just pressed
        swMidState = Pressed;
        //PT1AD1 ^= (LED_RED | LED_GREEN);
      }
      else
      { //Switch already pressed
        swMidState = Held;
      }
    }
    else
    { //Switch inactive - LOW level
      if (swMidState == Held)
      { //Switch was just released
        swMidState = Released;
        PT1AD1 ^= (RED_LED | GREEN_LED);
      }
      else
      {
        swMidState = Idle;
      }
    }
  }

    if (PT1AD1 & SW_RIGHT) //check if the right switch is pressed
    {
      //Sets the Green LED

      PT1AD1 |= GREEN_LED;
      PT1AD1 &= ~RED_LED;
      DDRM |= DDRP_DDRP7_MASK;
      PTM |= DDRP_DDRP7_MASK;
    }
    else
    {
      if (PT1AD1 & SW_MID)
      {
        PT1AD1 |= YELLOW_LED;
        DDRM &= ~DDRP_DDRP7_MASK;
        PTM &= ~DDRP_DDRP7_MASK;
      }
      else
      {

        PT1AD1 &= ~YELLOW_LED;
      }
      PT1AD1 &= ~GREEN_LED;
    }
  }
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
