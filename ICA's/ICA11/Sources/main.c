/********************************************************************/
// HC12 Program:  ECT COmpare Mode demo
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Samuel Tegegne
// Details:       Using ECT to create accurate time intervals
                  
// Date:     March 12th, 2021     

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
#include "SCI0_Lib.h"
#include "LCD_Lib.h"
#include "ECT.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/
void Timer_msTick(void);
/********************************************************************/
// Global Variables
/********************************************************************/
int msCounter, secCounter=0 ;
unsigned int ECTcounter=0;
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
  HAL_RtiInit(Timer_msTick);
  EnableInterrupts;

  /********************************************************************/
  // initializations
  /********************************************************************/
  SwLED_Init();
  SevSeg_Init();
  LCD_Init();
  SevSeg_Top4(0);
  SevSeg_Bot4(0);

  Timer_Init(Timer_Prescale_16);                           
    
  
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    Timer_Sleep_ms(100);
    SevSeg_Bot4(HexToBCD(++ECTcounter));
    if(ECTcounter>9999)
    {
      ECTcounter=0;
    } 
    
  }
}


/********************************************************************/
// Functions
/********************************************************************/

void Timer_msTick(void)
{
  if(++msCounter > 99)
  {
    msCounter = 0;
    ++secCounter;
    if(secCounter>9999)
    {
      secCounter=0;
    }
  SevSeg_Top4(HexToBCD(secCounter));
  
 }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
