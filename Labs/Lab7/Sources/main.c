/********************************************************************/
// HC12 Program:  PWM-3 Demo
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza Said
// Details:              
                  
// Date:    

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
#include "LCD_Lib.h"
#include "stdio.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/
void Timer_msTick(void);
/********************************************************************/
// Global Variables
/********************************************************************/
unsigned int msCounter;
char rtiUpdate;
unsigned int valueA;      // valus on pin 67 and 69 from ATD
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
  HAL_SetClk24();
  HAL_RtiInit(&Timer_msTick);
  EnableInterrupts;

  /********************************************************************/
  // initializations
  /********************************************************************/

  PWMPOL |= PWMPOL_PPOL7_MASK;
  PWMPRCLK= 0 ;
  PWMCTL |= PWMCTL_CON67_MASK;
  PWMPER67 = 512;
  PWMDTY67= valueA;
  PWME |= PWME_PWME7_MASK;

  //Power up - Fast flag clear - Enable interrupt
  ATD0CTL2 = ATD0CTL2_ADPU_MASK | ATD0CTL2_AFFC_MASK | ATD0CTL2_ASCIE_MASK;  

  //Number of conversion per sequence (1), finish current conversion then freeze
   ATD0CTL3 = ATD0CTL3_S1C_MASK ; 

  //10-bit resolution, 4 A/D coversion clock periods, prescaler to 1 -> 2MHZ ADC-Clock
  ATD0CTL4 =   ATD0CTL4_SMP0_MASK |ATD0CTL4_PRS0_MASK |ATD0CTL4_PRS2_MASK; 


  //Continuos conversion sequences, multi channel, right justified
  ATD0CTL5 = ATD0CTL5_DJM_MASK | ATD0CTL5_SCAN_MASK;

  SwLED_Init();
  SevSeg_Init();
  LCD_Init();

  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {

  }
}

/********************************************************************/
// Functions
/********************************************************************/
void Timer_msTick()
{
  if(++msCounter > 24)
  {//Second counter
    msCounter = 0;
    rtiUpdate = 1;    
  }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/

// A to D interrupt
interrupt VectorNumber_Vatd0 void Vatd0_Handler(void)
{
  // fast flag clear is enable
  valueA = ATD0DR0/2; // channel 0 value from pin 67
  PWMDTY67= valueA;
}
