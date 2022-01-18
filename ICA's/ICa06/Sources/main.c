 /********************************************************************/
// HC12 Program:  ICA 6
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza Said
// Details:       Store a user input and then display that on the terminal /
                  
// Date:   

/********************************************************************/
// Constant Defines
/********************************************************************/

/********************************************************************/
#include <hidef.h>      /* common defin es and macros */
#include "derivative.h" /* derivative-specific definitions */
/********************************************************************/
// Library includes
/********************************************************************/
// your includes go here
#include "hal.h"
#include "SwLED_Lib.h"
#include "SevSeg_Lib.h"
#include "SCI0_Lib.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/
void Timer_msTick(void);

/********************************************************************/
// Global Variables
/********************************************************************/
int msCounter = 0;

char rcvData;
//const char* message = "Welcome to the World of Serial Communication!";

enum SwState
{
  Idle,
  Pressed,
  Held,
  Released
};
enum SwState midState = Idle;

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
  HAL_RtiInit(&Timer_msTick);
  EnableInterrupts;


  /********************************************************************/
  // initializations
  /********************************************************************/
  HAL_Init();
  SwLED_Init();
  SevSeg_Init();
  SCI0_Init19200(NO_INT);
  //SCI0_Init115200(NO_INT);
  //SCI0_Init9600(NO_INT);
  
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    
  
    if(SCI0_RxChar(&rcvData))
    {
      SCI0_TxChar(rcvData);
      
    }


    if (PT1AD1 & SW_MID)
    {
      if (midState == Idle)
      {
        midState = Pressed;
        SCI0_TxString("\r\nWelcome to the world of serial communications!");
        SCI0_TxString("\n\r"); 
      }

      else 
      if(midState == Pressed)
      { 
        midState = Held;
      }
    }

    else
    {
        if ( midState == Held)
        {
          midState = Released;
        }
        else
        if(midState == Released)
          {
            midState = Idle;
          }

    }

  }                   
}


/********************************************************************/
// Functions
/********************************************************************/
void Timer_msTick()
{
  if(++msCounter > 1000)
  {
    msCounter = 0;
    
  }
}

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
