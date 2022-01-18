/********************************************************************/
// HC12 Program:  Number Manupulation using SCI
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Hamza Said
// Details:       A more detailed explanation of the program is entered here

// Date:          Oct 25th

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
#include "stdio.h"
#include "Math.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/
void Timer_msTick(void);
void Calculation(void);
/********************************************************************/
// Global Variables
/********************************************************************/
int msCounter, secCounter;
char update = 0;

//variables to hold the operands and the result
int numOne;
int numTwo;
int resultNum;
unsigned char message[16];
unsigned char answer;

enum calculationState
{
  Menu,
  WaitForNumOne,
  WaitForNumTwo,
  WaitForResult,
  OperationDone //Result is calculated at this point
};

enum calculationState mainState = Menu;

//buffer handler
struct BufferHandler sci0BufferHandler;
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
  HAL_Init();
  SwLED_Init();
  SevSeg_Init(); 
  //HAL_RtiInit(&Timer_msTick);
  SCI0_Init9600(INT);
  SCI_BufferInit(&sci0BufferHandler, CharSync, '\r'); //char syncing
  //Initialize the SCI0 module and set the baudeate to 19200

  /********************************************************************/

  
  
  /********************************************************************/
  // main program loop
  /********************************************************************/

  for (;;)
  {

    if (sci0BufferHandler.CommComplete)
    {

      switch (mainState)
      {
      case Menu:

        SCI0_TxString("\x1b[?25l");   //cursor off
        SCI0_TxString("\x1b[2J");   //Clear the screen
        SCI0_TxString("\x1b[0;0H"); // direct to Home
        SCI0_TxString("Basic Calculator\r\n");
        SCI0_TxString("\x1b[32m");
        SCI0_TxString("\tA = ");

        mainState = WaitForNumOne;

        break;

      case WaitForNumOne:

        //Pass the address of numOne in the sscanf method

        // SCI0_TxString("\tA = ");
        sscanf(sci0BufferHandler.Buffer, "%d", &numOne);
        sprintf(message, "%d", numOne);
        SCI0_TxString(message);
        SCI0_TxString("\r\n\tB = ");

        //IMPORTANT: Then you MUST clear the buffer because only 1 byte can be buffered at a time!!
        //SCI_BufferClear(&sci0BufferHandler);

        //Now ready to get the second number
        mainState = WaitForNumTwo;

        break;

      case WaitForNumTwo:
        //If a number is entered

        sscanf(sci0BufferHandler.Buffer, "%d", &numTwo);
        sprintf(message, "%d", numTwo);
        SCI0_TxString(message);
        SCI0_TxString("\r\nSelect Operation: \r\n");
        SCI0_TxString("\tAdd(1)\r\n");
        SCI0_TxString("\tSub(2)\r\n");

        mainState = WaitForResult;

        break;

      case WaitForResult:

        // If the\r received??

        // \r has been detected
        SCI0_TxString("\x1b[2K"); //Clear the current terminal line
        if (sci0BufferHandler.Buffer[0] == '1')
        {
          resultNum = numOne + numTwo;
        }
        else if (sci0BufferHandler.Buffer[0] == '2')
        {
          resultNum = numOne - numTwo;
        }

        //IMPORTANT:  MAKE sure to wait again here to get the response of the above

        SCI0_TxString("\x1b[37m");
        //  resultNum= numOne+numTwo;
        SCI0_TxString("\r\nThe result is ");
        sprintf(message, "%d", resultNum);
        SCI0_TxString(message);

        SCI0_TxString("\r\nPress Enter to restart\r\n");
        mainState = OperationDone;

        break;

      case OperationDone:

        mainState = Menu;

        break;

      default:
        break;
      }
      SCI_BufferClear(&sci0BufferHandler);
    }
  }
}

/********************************************************************/
// Functions
/********************************************************************/

void Timer_msTick()
{
  if (++msCounter > 1000)
  {
    msCounter = 0;
    ++secCounter;
    update = 1;
  }
}

void Calculation(void)
{
  //The main display
  switch (mainState)
  {
  case Menu:

    SCI0_TxString("\x1b[2J");   //Clear the screen
    SCI0_TxString("\x1b[0;0H"); // direct to Home

    SCI0_TxString("\x1b[32m");
    SCI0_TxString("\tA = ");

    SCI0_TxString("\x1b[2J");   //Clear the screen
    SCI0_TxString("\x1b[0;0H"); // direct to Home

    mainState = WaitForNumOne;

    break;

  case WaitForNumOne:
    if (sci0BufferHandler.CommComplete)
    {

      sscanf(sci0BufferHandler.Buffer, "%d", &numOne);
      sprintf(message, "%d", numOne);
      SCI0_TxString(message);
      SCI0_TxString("\r\n\tB = ");

      //clear the buffer because only 1 byte can be buffered at a time!!
      SCI_BufferClear(&sci0BufferHandler);

      //get the second number
      mainState = WaitForNumTwo;
    }
    break;

  case WaitForNumTwo:
    

    if (sci0BufferHandler.CommComplete)
    { //Carriage return received
      sscanf(sci0BufferHandler.Buffer, "%d", &numTwo);
      sprintf(message, "%d", numTwo);
      SCI0_TxString(message);
      SCI0_TxString("\r\nSelect Operation: \r\n");
      SCI0_TxString("\tAdd(1)\r\n");
      SCI0_TxString("\tSub(2)\r\n");
      SCI_BufferClear(&sci0BufferHandler);
      mainState = WaitForResult;
    }

    break;

  case WaitForResult:

    // If the\r received??
    if (sci0BufferHandler.CommComplete)
    {
      // \r has been detected
      SCI0_TxString("\x1b[2K"); //Clear the current terminal line
      if (sci0BufferHandler.Buffer[0] == '1')
      {
        resultNum = numOne + numTwo;
      }
      else if (sci0BufferHandler.Buffer[0] == '2')
      {
        resultNum = numOne - numTwo;
      }
    }

    //IMPORTANT:  MAKE sure to wait again here to get the response of the above
    if (sci0BufferHandler.CommComplete)
    {
      SCI0_TxString("\x1b[37m");
      //  resultNum= numOne+numTwo;
      SCI0_TxString("\r\nThe result is ");
      sprintf(message, "%d", resultNum);
      SCI0_TxString(message);
      SCI_BufferClear(&sci0BufferHandler);
      SCI0_TxString("\r\nPress Enter to restart\r\n");
      mainState = OperationDone;
    }
    break;

  case OperationDone:
    
    if (sci0BufferHandler.CommComplete)
    {
      //user input
      SCI_BufferClear(&sci0BufferHandler);
      mainState = Menu;
    }
    break;

  default:
    break;
  }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
//SCI0 Interrupt------------------------------------
interrupt VectorNumber_Vsci0 void Vsci0_Handler(void)
{
  SCI0SR1 |= SCI0SR1_RDRF_MASK; //Clear Flag, Has to happen via interrupt

  SCI0_AddToBuffer(&sci0BufferHandler, SCI0DRL);
}