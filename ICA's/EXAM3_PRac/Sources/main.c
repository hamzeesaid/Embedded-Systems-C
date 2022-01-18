/********************************************************************/
// HC12 Program:  Exam 3 Winter 2021 Solution 
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Carlos Estay
// Details:       Exam 3 Winter 2021 Solution 
                  
// Date:          December 11th, 2021   
// Revision History : 


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
#include "ECT.h"
#include "IIC0_Lib.h"
#include "stdio.h" //for sprintf

/********************************************************************/
// Local Prototypes
/********************************************************************/
void TickFunction(void);

/********************************************************************/
// Global Variables
/********************************************************************/
int msCounter = 0;
char rtiUpdate = 0; //it should happen every 1 sec
unsigned int conversions[8];
int test;
unsigned int number;
unsigned int voltage1; //to be displayed on LCD

unsigned int dacValue;

char Message[40];
char dacUpdate;




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
  EnableInterrupts;
  /********************************************************************/
  // initializations
  /********************************************************************/
  HAL_RtiInit(TickFunction);
  SwLED_Init();
  SevSeg_Init();  
  LCD_Init();

  //ADC Configuration

  // //Power up - Fast flag clear - Interrupt enable
  ATD0CTL2 = ATD0CTL2_ADPU_MASK | ATD0CTL2_AFFC_MASK | ATD0CTL2_ASCIE_MASK;

  //Number of conversion per sequence (8), Continuous conversion, no freeze 
  ATD0CTL3 = 0;
  //ATD0CTL3 = ATD0CTL3_S8C_MASK; //either way works

  //10-bit resolution, 4 A/D coversion clock periods, prescaler to 1 -> 2MHZ ADC-Clock
  ATD0CTL4 = ATD0CTL4_SMP0 | ATD0CTL4_PRS0_MASK; 

  //ATD0CTL0 = 0;// We do not need to wrap, this was the problem
  //We need to wrap around AN7, so it does all 8 channels
  ATD0CTL0 = ATD0CTL0_WRAP2_MASK | ATD0CTL0_WRAP1_MASK | ATD0CTL0_WRAP0_MASK;


  //Right align, multiple conversion, continuous conversion sequence mode
  ATD0CTL5 =  ATD0CTL5_DJM_MASK | ATD0CTL5_MULT_MASK | ATD0CTL5_SCAN_MASK;   
  
  //Timer configuration;
  Timer_Init(Timer_Prescale_8);


  //Channel 0 config. Output COmpare
  TimerCH_EnableAsOutput(Timer_CH0);
  TimerCH_IntEnable(Timer_CH0);

  /* Compare result Action - TCTL1/TCTL2
  OMx   OLx
  0     0  Timer Disconnected from pin
  0     1  Toggle OCx output line
  1     0  Clear OCx output lize to zero
  1     1  Set OCx output line to one
  *****************************************/
  TCTL2_OM0 = 0;
  TCTL2_OL0 = 1;
  TC0 = 500;


  //LCD_CursOff();

  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    if(dacUpdate)
    {
      dacUpdate = 0;
      SendDac(dacValue, DAC_ALL);

    }
    
    if(rtiUpdate)
    {
      rtiUpdate = 0; 
      LCD_Pos(0,0); 
      sprintf(Message, "%04d [mV]", voltage1);
      LCD_String(Message);      
    }
  }
}

/********************************************************************/
// Functions
/********************************************************************/
void TickFunction(void)
{//every 1 [ms]
  if(++msCounter > 999)
  {
    msCounter = 0;
    rtiUpdate = 1;
    
  }
}
//------------------------------------------------------------------------------

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
//----------------------------------------AtoD Interrupt
interrupt VectorNumber_Vatd0 void Vatd0_Handler(void)
{
  conversions[0] = ATD0DR0;
  conversions[1] = ATD0DR1;  
  conversions[2] = ATD0DR2;
  conversions[3] = ATD0DR3; 
  conversions[4] = ATD0DR4;
  conversions[5] = ATD0DR5;  
  conversions[6] = ATD0DR6;
  conversions[7] = ATD0DR7; 


  75% -> 767
  25% -> 256
  This is for DAC
  if(conversions[0] > 767)
  {
    dacValue = 4095;
  }
  else if(conversions[0] > 256)
  {
    dacValue = 2048;
  }
  else
  {
    dacValue = 0;
  }

  //This is for LCD
  voltage1 = conversions[0] * 5;
}
//----------------------------------------TC0 Interrupt
interrupt VectorNumber_Vtimch0 void Vtimch0_Handler(void)
{
  //We need to clear thge flag like this if the TSCR1_TFFCA bit is not set
  //in your TSCR1 register (fast flag clear). MAke sure you add this to your initialization
  //To avoid doing this flag clearing
  // TFLG1 = TFLG1_C0F_MASK; 
  TC0 += 500;
  dacUpdate = 1;
  


}
// //----------------------------------------TC7 Interrupt
// interrupt VectorNumber_Vtimch7 void Vtimch7_Handler(void)
// {
//   //We need to clear thge flag like this if the TSCR1_TFFCA bit is not set
//   //in your TSCR1 register (fast flag clear). MAke sure you add this to your initialization
//   //To avoid doing this flag clearing
//   //TFLG1 = TFLG1_C7F_MASK; 
//   periodCounts = TC7 - lastEdge;
//   lastEdge = TC7;
//   ++edgeCounts;
// }
