/*
* File:   display.c
* Author: HP
*
* Created on October 26, 2025, 10:32 PM
*/
 
 
#include "xc.h"
#include "lcd.h"
#include "stdbool.h"
#include <stdio.h>   // for sprintf
#include <string.h>  // for memset
 
 
/*******************************************************************************
 
  Function:
    void Delay_ms (unsigned int x)
 
  Summary:
    Delay needed for the LCD to process data (in ms)
*/
void Delay_ms(unsigned int x)
{
    TMR2 = 0;
    while (TMR2_CounterGet() < (389 * x)); /* 40 Timer 1 ticks = ~1ms */
}
 
 
/*******************************************************************************
 
  Function:
    void initializeLCD (void)
 
  Summary:
    LCD initialization sequence
*/
void initializeLCD(void)
{
    /* Configure general PMP options - enable read/write strobes, long waits, etc */
    //PMP_AddressSet(0x1);
    //PMP_AddressPortEnable(0x1);
 
    //Delay_ms(60); /* LCD needs 60ms to power on and perform startup functions */
 
    PMP_AddressSet(CMDREG); /* Access the LCD command register */
 
    PMP_MasterSend( 0x38); /* LCD Function Set - 8-bit interface, 2 lines, 5*7 Pixels */
    Delay_ms(1); /* Needs a 40us delay to complete */
 
    PMP_MasterSend( 0x0C); /* Turn on display (with cursor hidden) */
    Delay_ms(1); /* Needs a 40us delay to complete */
 
    PMP_MasterSend( 0x01); /* Clear the display */
    Delay_ms(2); /* Needs a 1.64ms delay to complete */
 
    PMP_MasterSend( 0x06); /* Set text entry mode - auto increment, no shift */
    Delay_ms(1); /* Needs a 40us delay to complete */
}
 
 
/*******************************************************************************
 
  Function:
    void writeToLCD(int reg, char c)
 
  Summary:
    Writes a byte of data to either of the two LCD registers (DATAREG, CMDREG)
*/
void writeToLCD(int reg, char c)
{
    PMP_AddressSet(reg); /* Either 'DATAREG' or 'CMDREG' */
 
    PMP_MasterSend(c);
    while(PMP_PortIsBusy() == true);
    Delay_ms(1); /* 40us needed in between each write */
}
 
 
/*******************************************************************************
 
  Function:
    void writeString (unsigned char *string)
 
  Summary:
    Used to write text strings to the LCD
*/
void writeString(char *string)
{
    while (*string)
    {
        writeToLCD(DATAREG, *string++); /* Send characters one by one */
    }
}
 
 
/*******************************************************************************
 
  Function:
    void newLine(void)
 
  Summar0xFFFFy:
    Sets the LCD cursor position to line two
*/
void newLine(void)
{
    writeToLCD(CMDREG, 0xC0); /* Cursor address 0x80 + 0x40 = 0xC0 */
}
 
 
void displayTemperature(double temp)
{
    char buffer[16];
 
    /* Clear display */
    writeToLCD(CMDREG, 0x01);
    Delay_ms(2);
 
    /* Line 1 */
    writeString("Temp:");
 
   
 
    /* Format temperature */
    /* Example output: 25.37 C */
    sprintf(buffer, "%.2f C", temp);
 
    writeString(buffer);
}/*
* File:   display.c
* Author: HP
*
* Created on October 26, 2025, 10:32 PM
*/
