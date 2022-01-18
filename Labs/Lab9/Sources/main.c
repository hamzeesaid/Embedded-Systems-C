/********************************************************************/
// HC12 Program:  Lab 9
// Processor:     MC9S12XDP512
// Bus Speed:     8MHZ
// Author:        Hamza Said
// Details:       Thermometer/Barometer
// Date:          Dec 2021
// Revision History :
//

/********************************************************************/
/********************************************************************/
// Library includes
/********************************************************************/
// your includes go here
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "hal.h"
#include "SwLED_Lib.h"
#include "SevSeg_Lib.h"
#include "SCI0_Lib.h"
#include "LCD_Lib.h"
#include "ECT.h"
#include "IIC0_Lib.h"
#include "stdio.h"
#include "string.h"
/********************************************************************/
// Local Prototypes
/********************************************************************/
float TempCalc(char *);
float PressCalc(char *);
void Timer_msTick(void);
/********************************************************************/
// Global Variables
/********************************************************************/
//Lookup table
unsigned int msCounter;
char rtiUpdate;
// unsigned int rtiUpdate; 
unsigned int value, value2;
int test;
char readData;
char dataArray[5];

char message[24];

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

  LCD_Init();
  Cursor_Off();
  IIC0_InitBus(I2CBus400);

  //PIT TIMER 1 Settings,
  PITLD1 = 3199;                  //Set counter1 to Mod-Down-3200 -> 400[us] -> 2500[hz]
  PITINTE |= PITINTE_PINTE1_MASK; //interrupt enable PIT1
  PITCE |= PITCE_PCE1_MASK;       //enable PIT channel 1

  //Configuring the MPL3115A2: "Precision Altimeter"

  //CTRL_REG1(0x26) -> 0b00111000 - Barometer mode, 128 Over Sampling, verything else 0
  test = IIC0_RegWrite(MPL3115A2_ADD, CTRL_REG1, 0x38, IIC0_STOP);

  //PT_DATA_CFG(0x13) -> 0b00000111 - Data ready Event flag, Event or Pressure/Alt, Event on Temp
  test = IIC0_RegWrite(MPL3115A2_ADD, PT_DATA_CFG, 0x07, IIC0_STOP);

  //Activate device (out of stand-by mode)
  //CTRL_REG1(0x26) -> 0b00111001
  test = IIC0_RegWrite(MPL3115A2_ADD, CTRL_REG1, 0x39, IIC0_STOP);

  

  /*
  Registers to read:
  OUT_P_MSB (0x01) -> Pressure data out MSB
  OUT_P_CSB (0x02) -> Pressure data out CSB
  OUT_P_LSB (0x03) -> Pressure data out LSB
  OUT_T_MSB (0x04) -> Temperature data out MSB
  OUT_T_LSB (0x05) -> Temperature data out LSB
  */
  readData = 0;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  LCD_Pos(0, 0);
  LCD_String("Current Conditions: ");
  for (;;)
  {
    if (rtiUpdate)
    {
      while (!(readData & DR_STATUS_PTDR))
      {
        test = IIC0_RegRead(&readData, MPL3115A2_ADD, DR_STATUS);
      }
      if (readData & DR_STATUS_PTDR)
      {
        test = IIC0_RegRead(&dataArray[0], MPL3115A2_ADD, 0x01);
        test = IIC0_RegRead(&dataArray[1], MPL3115A2_ADD, 0x02);
        test = IIC0_RegRead(&dataArray[2], MPL3115A2_ADD, 0x03);
        test = IIC0_RegRead(&dataArray[3], MPL3115A2_ADD, 0x04);
        test = IIC0_RegRead(&dataArray[4], MPL3115A2_ADD, 0x05);

        if (dataArray[3] < 0)
        {
          LCD_Pos(2, 0);
          test = sprintf(message, "Temp: -%0.2fC", TempCalc(&dataArray[3]));
          LCD_String(message);
          LCD_Pos(2, 11);
          LCD_Char(0b11011111);
        }

        else
        {
          LCD_Pos(2, 0);
          test = sprintf(message, "Temp: +%0.2fC", TempCalc(&dataArray[3]));
          LCD_String(message);
          LCD_Pos(2, 11);
          LCD_Char(0b11011111);
        }

        LCD_Pos(3, 0);
        test = sprintf(message, "Press: %.1f kPa", PressCalc(&(dataArray[0])));
        LCD_String(message);
        readData = 0;
      }
      rtiUpdate = 0;
    } //End of if(update)
  }   //End of Infinite loop (for)
} //End of Main

/********************************************************************/
// Functions
/********************************************************************/
void Timer_msTick()
{
  if(++msCounter > 9)
  {//Second counter
    msCounter = 0;
    rtiUpdate = 1;    
  }
}
//-----------------------------------------------------Funtion to calculate Temperature
float TempCalc(char *data)
{
  float value, decFactor;
  int tempInt, tempDec;
  int index;
  /*
  decimal part
  2^-4 = 0.0625
  2^-3 = 0.125
  2^-2 = 0.25
  2^-1 = 0.5
  */
  tempInt = (char)data[0]; //First byte contains the integer part
  tempDec = (char)data[1]; //second byte contains the decimal part
  tempDec >>= 4;           //4 decimal  precision, 4 LSB are don't care

  value = tempInt;
  decFactor = 0.0625;
  for (index = 1; index <= 4; index++)
  {
    if (tempDec & 0x1)
    {
      if (tempInt < 0)
      {
        value -= decFactor; //Temp is negative, subtract decimal part to value
      }
      else
      {
        value += decFactor; //add decimal part to value
      }
    }
    decFactor *= 2;
    tempDec >>= 1;
  }

  return value;
}
//-----------------------------------------------------Funtion to calculate Pressure
float PressCalc(char *data)
{
  unsigned long pressInt = 0; //to hold integer part
  unsigned char temp = 0;     //to hold LSB and shifting
  float value = 0;            //final value to be returned

  pressInt = (unsigned char)data[0]; //get MSB
  pressInt <<= 8;                    //shift to levae space for CSB
  pressInt |= (unsigned char)data[1];

  temp = data[2]; //get LSB
  temp >>= 4;     //shift out reserved part

  if (temp & 0x1)
  {
    value += 0.5; //add decimal part to value (2^-1)
  }
  temp >>= 1; //shift out decimal bit
  if (temp & 0x1)
  {
    value += 0.25; //add decimal part to value (2^-2)
  }
  temp >>= 1; //shift out decimal bit

  pressInt <<= 2; //Make room for last 2 bits (INT part)
  pressInt |= temp;

  value += pressInt; //Add integer part to final value

  return (value/1000);
}

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
//Periodic Interrupt timer interrupt
 interrupt VectorNumber_Vpit1 void Vpit1_Handler(void)
 {
  PITTF |= PITTF_PTF1_MASK; //clear flag;
  PTT ^= PTT_PTT7_MASK;
 }
