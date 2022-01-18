/********************************************************************/
// HC12 Program:  Dual DAC Demo
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza Saud
// Details:       DAC Demo                 
                  
// Date:        

/********************************************************************/
// Constant Defines
/********************************************************************/
#define MAX_RGB_DC 100
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include <stdlib.h>
#include <stdio.h>
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
#include "IIC0_Lib.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/
void Timer_msTick(void);
/********************************************************************/
// Global Variables
/********************************************************************/

unsigned int msCounter;
unsigned int value, value2;
char rtiUpdate;
unsigned int atdVal;      // values from the AtoD(pin 67)

double volIn;             // calculated voltage from AtoD
double volOut;            // voltage sent to DAC
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
  

    //SwLED_Init();
  SevSeg_Init();
  LCD_Init();
  Cursor_Off();
  
  
    //Power up - Fast flag clear - Enable interrupt
  ATD0CTL2 = ATD0CTL2_ADPU_MASK | ATD0CTL2_AFFC_MASK;  

  //Number of conversion per sequence (1), finish current conversion then freeze
   ATD0CTL3 = ATD0CTL3_S1C_MASK ; 

  //10-bit resolution, 4 A/D coversion clock periods, prescaler to 1 -> 2MHZ ADC-Clock
  ATD0CTL4 =   ATD0CTL4_SMP0 | ATD0CTL4_PRS0_MASK; 


  //Continuos conversion sequences, multi channel, right justified
  ATD0CTL5 = ATD0CTL5_DJM_MASK | ATD0CTL5_SCAN_MASK;

    //DAC Configuration
  //IIC0_Init();
  IIC0_InitBus(I2CBus400);
  

  //PIT TIMER 0 Settings, 
  PITLD1  = 3999;  //Set counter0 to Mod-Down-4000-> 500[us] -> 2000[hz]
  PITINTE |= PITINTE_PINTE1_MASK;                   //interrupt enable PIT1
  PITCE |= PITCE_PCE1_MASK;                         //enable PIT channel 1



  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    if(rtiUpdate)
    {
      //Reset RIT Update  
      rtiUpdate = 0;
    }
  }
}

/********************************************************************/
// Functions
/********************************************************************/
void Timer_msTick()
{
  if(++msCounter > 89)
  {//Second counter
    msCounter = 0;
    rtiUpdate = 1;    
  }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
//Periodic Interrupt timer interrupt
interrupt VectorNumber_Vpit1 void Vpit1_Handler(void)
{
  PITTF |= PITTF_PTF1_MASK; //clear flag;
  // get the value from ATD channel 0(pin 67)
  atdVal = ATD0DR0;

  // calculated value (step size 5mv = 0.005)
  volIn = atdVal * 0.005;

  // new vOut value (scale = DAC/AtD)
  // transfer function from the range of A to D values to the range of DAC values
  volOut  = volIn *(4.095/5.115);

  // value to write on DAC in Volts
  SendDac(volOut*1000, DAC_A);
 //test = IIC0_WriteDAC(value, DAC_ALL);

 SendDac(-(volOut*1000), DAC_B);
 
}
