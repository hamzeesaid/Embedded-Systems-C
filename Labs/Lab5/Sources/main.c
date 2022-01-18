
/********************************************************************/
// HC12 Program:  Lab5- ASCII Number Manipulation
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza Said
// Details:       
                  
// Date:          12/11/2021

/********************************************************************/

/********************************************************************/
// Library includes
/********************************************************************/


#include <hidef.h> /* common defines and macros */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "derivative.h" /* derivative-specific definitions */
#include "LCD_Lib.h"
#include "SwLED_Lib.h"
#include "SevSeg_Lib.h"
#include "SCI0_Lib.h"
#include "hal.h"
#include "BufferHandler.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/
void Timer_msTick(void);
void userInterface(char*);
/********************************************************************/
// Global Variables
/********************************************************************/
char rawString[] = "This2iS7a4sTRING!";   // provided string
int msCounter, secCounter;
char update=0;
int i;                                    // to iterate through each string char
int total = 0;       // to store final total as integer
char numString[19];  // to store converted total as char string to display on lcd
BufferHandler bufferA;
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
  HAL_RtiInit(&Timer_msTick);  
  HAL_SetClk24();
  EnableInterrupts;
  
  /********************************************************************/
  // initializations
  /********************************************************************/
  SCI0_Init19200(INT);
  SwLED_Init();
  LCD_Init();
  Buffer_Init(&bufferA, CharSync, '\r'); //char syncing
  Cursor_Off();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  userInterface(rawString);
  for (;;)
  {
    if(bufferA.RxComplete)
    {

      userInterface(bufferA.Buffer);
      Buffer_Clear(&bufferA);
    }
  }
}

/********************************************************************/
// Functions
/********************************************************************/
void Timer_msTick()
{
  if(++msCounter > 999)
  {
    msCounter = 0;
    ++secCounter;
    update = 1;
  }
}

void userInterface(char userInput[])
{  
  int index;
  int total = 0;       // to store final total as integer
  char numString[19];  // to store converted total as char string to display on lcd
  LCD_Clear();
  

  for (index = 0; userInput[index] != '\0'; index++)
  {
    if (userInput[index] >= '0' && userInput[index] <= '9')
    {
      total += (userInput[index] - '0');
    }
    (void)sprintf(numString,"0x%02X",total);
  }

  //Display in Hex
   LCD_Pos(2, 8);
   LCD_String(numString);
  //Display in Dec
   LCD_Pos(3, 8);
   (void)sprintf(numString,"%d",total);
   LCD_String(numString);
  
   
  // replace num  with space(Decimal value 32 in ASCii)
  // display all in uppercase
  for (index = 0; userInput[index] != '\0'; index++)
  {
    if (userInput[index] >= '0' && userInput[index] <= '9')
    {
      userInput[index] = 32;
    }

    if (userInput[index] >= 'a' && userInput[index] <= 'z')
    {
      
      //If lowercase alphabet then
      //convert it to uppercase.
        
      userInput[index] = userInput[index] - 32;    
      userInput[index] = (userInput[index]);
    }
  }
  LCD_Pos(0, 0);
  LCD_String(userInput);
  

  // display in lowercase
  for (index = 0; userInput[index] != '\0'; index++)
  {
    if (userInput[index] >= 'A' && userInput[index] <= 'Z')
    {

      userInput[index] = userInput[index] + 32;
      userInput[index] = (userInput[index]);
    }
  }
  LCD_Pos(1, 0);
  LCD_String(userInput);
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
//SCI0 Interrupt------------------------------------
interrupt VectorNumber_Vsci0  void Vsci0_Handler(void)
{
  SCI0SR1 |= SCI0SR1_RDRF_MASK; //Clear Flag
    Buffer_Rx(&bufferA, SCI0DRL);
} 