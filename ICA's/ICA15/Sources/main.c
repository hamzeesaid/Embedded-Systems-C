/********************************************************************/
// HC12 Program:  PWM Demo
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza Said
// Details:                     
                  
// Date:      

/********************************************************************/
// Constant Defines
/********************************************************************/
#define NUMBER_OF_FREQ 8
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
unsigned int msCounter;
char update;
int countsArrayIndex = 0;

unsigned char CountsArray2[NUMBER_OF_FREQ] = {239, 213, 190};


// const unsigned int PWM256[] = { 128,131,134,137,140,143,146,149,152,155,158,161,164,167,170,173,
//                                 176,178,181,184,187,189,192,194,197,199,202,204,207,209,211,213,
//                                 216,218,220,222,224,225,227,229,231,232,234,235,237,238,239,240,
//                                 242,243,244,245,245,246,247,248,248,249,249,249,250,250,250,250,
//                                 250,250,250,249,249,249,248,248,247,246,245,245,244,243,242,240,
//                                 239,238,237,235,234,232,231,229,227,225,224,222,220,218,216,213,
//                                 211,209,207,204,202,199,197,194,192,189,187,184,181,178,176,173,
//                                 170,167,164,161,158,155,152,149,146,143,140,137,134,131,128,125,
//                                 122,119,116,113,110,107,104,101, 98, 95, 92, 89, 86, 83, 80, 77,
//                                 74 , 72, 69, 66, 63, 61, 58, 56, 53, 51, 48, 46, 43, 41, 39, 37,
//                                 34 , 32, 30, 28, 26, 25, 23, 21, 19, 18, 16, 15, 13, 12, 11, 10,
//                                 8	 ,  7,  6,  5,  5,  4,  3,  2,  2,  1,  1,  1,  0,  0,  0,  0,
//                                 0	 ,  0,  0,  1,  1,  1,  2,  2,  3,  4,  5,  5,  6,  7,  8, 10,
//                                 11 , 12, 13, 15, 16, 18, 19, 21, 23, 25, 26, 28, 30, 32, 34, 37,
//                                 39 , 41, 43, 46, 48, 51, 53, 56, 58, 61, 63, 66, 69, 72, 74, 77,
//                                 80,  83, 86, 89, 92, 95, 98,101,104,107,110,113,116, 19,122,125,
//                                 };

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

  //PWM Initialization
  PWMPRCLK = 0b01100000;  //Set Clock B prescaler to / 64 -> 8[us] /Tick
  PWMCLK = 0; //Set to CLock source A/B
  PWMPOL |= PWMPOL_PPOL6_MASK; //PWM6 and PWM3 to High polarity, at the beginning of the period
  PWMDTY6 = PWMPER6 / 2;  //Define Pulse width
  PWME &= ~PWME_PWME6_MASK;  //Enable channel 6(speaker)

  //Por J initialization
  DDRJ &= ~(PTJ_PTJ1_MASK | PTJ_PTJ0_MASK); //Make PJ1 and PJ0 inputs
  PIEJ |= PTJ_PTJ1_MASK | PTJ_PTJ0_MASK;  // Enable Interrupt for PJ1 and PJ0
  PPSJ |= PTJ_PTJ1_MASK | PTJ_PTJ0_MASK;  //Rising edge for PJ1 and PJ0 


  SwLED_Init();
  SevSeg_Init();
  LCD_Init();
  Cursor_Off();

  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {


    //Update Counters and meesages every second
    if(update)
    {
      //Reset RIT Update  
      update = 0;  
    //Change Duty cycle
    if(PT1AD1 &SW_UP)
    {
      //increase DC
        if(++PWMDTY6 >PWMPER6-2)
        {
          PWMDTY6 = PWMPER6-2;
        }  
    }
    else if(PT1AD1 &SW_DOWN)
    {
      //decrease DC
        if(--PWMDTY6 < 2)
        {
          PWMDTY6 = 2;
        }       

    }
      if(PT1AD1 & SW_LEFT)
      {

      PWME |= PWME_PWME6_MASK;  //Enable channel 6(speaker) 
        PWMPER6=239;    //period counts //C5

      }
      else if(PT1AD1 & SW_RIGHT)
      {

      PWME |= PWME_PWME6_MASK;  //Enable channel 6(speaker)
      PWMPER6=190;          //period counts //D5

      }
      else if (PT1AD1 & SW_MID)
      {
      PWME |= PWME_PWME6_MASK;  //Enable channel 6(speaker)
        PWMPER6=213;          //period counts E5
      }
    }
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
    update = 1;    
  }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vportj void Vportj_Handler(void)
{
  if(PIFJ & PIFJ_PIFJ0_MASK)
  {

    PWME ^= PWME_PWME6_MASK;      //toggles channel 6(speaker)
    PIFJ |= PIFJ_PIFJ0_MASK;       //clear flag if active
  }
  PWMDTY6 = PWMPER6 / 2;
}
