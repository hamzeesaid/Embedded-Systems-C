/********************************************************************/
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza Said
// Details:

// Date:          8/10/2021
// Revision History :
//  each revision will have a date + desc. of changes


#include <hidef.h>      
#include "derivative.h" 


/********************************************************************/
// Library includes
/********************************************************************/

// your includes go here
//#include "misc.h"
//#include "muxled.h"
#include "SevSeg_Lib.h"
#include "hal.h"
#include "SwLED_Lib.h"
/********************************************************************/
// Local Prototypes
/********************************************************************/
void Delay_1s(void);

/********************************************************************/
// Global Variables
/********************************************************************/

// array fore HEX values
unsigned int hex[10] =
    {
        0,
        0x270f,
        0x2710,
        0x000f,
        0x00ff,
        0x0fff,
        0xffff,
        0x000a,
        0x0010,
        0xd321};

// array for BCD values
unsigned int dec[10] =
    {
        0,
        0x9999,
        0x10000,
        0x0015,
        0x0255,
        0x4095,
        0x00b1,
        0x0010,
        0x000a,
        0xabcd};

unsigned int i; // iterates through the loop and stores the array value
int k;          //iterates through the ms function
int l;
unsigned int *output;
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
    for (i = 0; i < 10; i++)
    {

      // display given HEX values on the Top Row
      SevSeg_Top4(hex[i]);

      // display converted BCD values on the Bottom Row
      SevSeg_Bot4(HexToBCD(hex[i]));

      // delay of 1 sec
      Delay_1s();
      
    }

      // delay of 3 sec for second function
      Delay_1s();

    for (i = 0; i < 10; i++)
    {
      // display given BCD values on the Top Row
      SevSeg_Top4(dec[i]);

      // HEX values on the Bottom Row
      SevSeg_Bot4(BCDToHex(dec[i]));

      Delay_1s();
    
      
    }
  }
}
                     
/********************************************************************/
// Functions
/********************************************************************/
void Delay_1s()
{
  for (k = 0; k < 100; k++)
  {
    Delay10ms_asm();
  }
}

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
