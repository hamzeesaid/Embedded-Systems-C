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
char update = 0;
int countsArrayIndex = 0;



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

 

  //For the LCD backlight
  PWMPOL |= PWMPOL_PPOL3_MASK ; //PWM3 to High polarity, 
  PWMPRCLK= PWMPRCLK_PCKB0_MASK|PWMPRCLK_PCKB1_MASK |PWMPRCLK_PCKA0_MASK|PWMPRCLK_PCKA1_MASK;
  PWMSCLB =25;
  PWMPER3 = 100 ;     // period
  PWMDTY3 = 50;       //OR PWMPER3 / 2 =50% duty cycle
  PWMCLK |=PWMCLK_PCLK3_MASK;
  PWME |= PWME_PWME3_MASK;  //Enable channel 3(LCD backlight)
  
  //For the RGB
  PWMCLK |=PWMCLK_PCLK0_MASK|PWMCLK_PCLK1_MASK|PWMCLK_PCLK4_MASK;
  PWMPOL |= PWMPOL_PPOL4_MASK| PWMPOL_PPOL1_MASK| PWMPOL_PPOL0_MASK; //PWM6 and PWM3 to High polarity, 
  PWMSCLA =10;
  PWMPER0 = 200;     // period
  PWMPER1= 200;
  PWMPER4=200;
  PWMDTY0 = 100;       //OR PWMPER3 / 2 =50% duty cycle
  PWMDTY1 = 100;       //OR PWMPER3 / 2 =50% duty cycle
  PWMDTY4 = 100;       //OR PWMPER3 / 2 =50% duty cycle
  PWME |= PWME_PWME0_MASK|PWME_PWME1_MASK|PWME_PWME4_MASK;  //Enable channel 3(LCD backlight)
  


  SwLED_Init();
  SevSeg_Init();
  LCD_Init();
  Cursor_Off();

  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {


    //Update every second
    if(update)
    {
      //Reset RIT Update  
      update = 0;  
      LCD_Pos(0,0);
      LCD_String("You light up");
      LCD_Pos(1,0);
      LCD_String("my life!");
    //Change Duty cycle
    if(PT1AD1 &SW_UP)
    {
      //increase Duty Cycle
        if(++PWMDTY3 >=99)
        {
          PWMDTY3 = 99;
        }  
    }
    else if(PT1AD1 &SW_DOWN)
    {
      //decrease Duty Cycle
        if(--PWMDTY3 <= 1)
        {
          PWMDTY3 = 1;
        }       

    }
      if(PT1AD1 & SW_LEFT)
      {

        if(++PWMDTY4 >=100)
        {
          PWMDTY4 = 100;
        }        

      }
      else 
      {
        if(--PWMDTY4 <=2)
        {
          PWMDTY4 = 2;
        }
      

      }
      if(PT1AD1 & SW_MID)
      {


        if(++PWMDTY1 >=100)
        {
          PWMDTY1 = 100;
        }        

      }
      else 
      {
        if(--PWMDTY1 <=2)
        {
          PWMDTY1 = 2;
        }
      

      }
      if(PT1AD1 & SW_RIGHT)
      {

        if(++PWMDTY0 >=100)
        {
          PWMDTY0 = 100;
        }        

      }
      else 
      {
        if(--PWMDTY0 <=2)
        {
          PWMDTY0 = 2;
        }
      

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
  {
    msCounter = 0;
    update = 1;
  }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/

