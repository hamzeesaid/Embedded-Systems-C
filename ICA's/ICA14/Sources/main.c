/********************************************************************/
// HC12 Program:  ADC Demo 2
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza Said
// Details:       microcontroller work as a two channel voltmeter
                  
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
#include "SCI0_Lib.h"
#include "LCD_Lib.h"
#include "ECT.h"
#include "stdlib.h"
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
unsigned int valueB;
char message[20];           // to print values to lcd
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
  //HAL_SetClk24();
  HAL_RtiInit(&Timer_msTick);
  EnableInterrupts;

  /********************************************************************/
  // initializations
  /********************************************************************/
    DDR1AD0 = 0;
    ATD0DIEN = 0;
   
    // power on             
    // fast flag clear
    // wait mode
     ATD0CTL2 = ATD0CTL2_ADPU_MASK | ATD0CTL2_AFFC_MASK | ATD0CTL2_AWAI_MASK;

    // enable interrupts on user selection
     ATD0CTL2 |= ATD0CTL2_ASCIE_MASK;
    
    // scan all 8 channels
    // finish current conversion then freeze
    ATD0CTL3 = ATD0CTL3_FRZ1_MASK; 
    
    // at max bus clock of 8Mhz, and for max ATD freq, 
    // prescale val should be set to 1
    ATD0CTL4 = ATD0CTL4_SMP0_MASK |ATD0CTL4_PRS0_MASK;
    
    // right justified     
    // continuous scan      
    // sample all 8 channels   
    ATD0CTL5 = ATD0CTL5_DJM_MASK | ATD0CTL5_SCAN_MASK | ATD0CTL5_MULT_MASK;

  SwLED_Init();
  SevSeg_Init();
  LCD_Init();
  Cursor_Off();

   

  /********************************************************************/
  // main program loop
  /********************************************************************/
  LCD_Pos(0, 0);
  LCD_String("Ch0 V =       V");
  LCD_Pos(1, 0);
  LCD_String("Ch1 V =       V");

  for (;;)
  {

    // update at every 250ms
    if (rtiUpdate)
    {
      
      // step size is 0.005V so multiply it with the received values, 
      // to get the actual analog values
      // and display on the LCD  
      sprintf(message, "%5.3f", valueA * 0.005);
      LCD_Pos(0, 8);
      LCD_String(message);
      sprintf(message, "%5.3f", valueB * 0.005);
      LCD_Pos(1, 8);
      LCD_String(message);

      // leds to shows the bigger value
      if(valueA > valueB)
      {
        LED_On('R');
        LED_Off('G');
      }
      else
      {
        LED_On('G');
        LED_Off('R');
      }
      rtiUpdate = 0;
    }
  } //End of Infinite loop (for)
} //End of Main


/********************************************************************/
// Functions
/********************************************************************/
void Timer_msTick()
{
  if(++msCounter > 249)
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
  valueA = ATD0DR0; // channel 0 value from pin 67
  valueB = ATD0DR1; // channel 1 value from pin 69
}

