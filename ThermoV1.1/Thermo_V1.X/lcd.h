/* ************************************************************************** */

/** Descriptive File Name
 
  @Company

    Company Name
 
  @File Name

    filename.h
 
  @Summary

    Brief description of the file.
 
  @Description

    Describe the purpose of this file.

*/

/* ************************************************************************** */
 
#ifndef _PRINT_H    /* Guard against multiple inclusion */

#define _PRINT_H
 
#include "config/default/peripheral/pmp/plib_pmp.h"

#include "config/default/peripheral/tmr/plib_tmr2.h"

#include "config/default/peripheral/tmr/plib_tmr_common.h"

// *****************************************************************************

// *****************************************************************************

// Section: Function Prototypes

// *****************************************************************************

// *****************************************************************************
 
void Delay_ms(unsigned int x);

void initializeLCD(void);

void writeToLCD(int reg, char c);

void writeString(char *string);

void newLine(void);

void displayTemperature(double temp);
 
 
// *****************************************************************************

// *****************************************************************************

// Section: Constant Data

// *****************************************************************************

// *****************************************************************************
 
/* Define aliases for the LCD data and command registers */

#define DATAREG 1 /* Data register */

#define CMDREG 0  /* Command register */
 
 
#endif /* _EXAMPLE_FILE_NAME_H */
 
/* *****************************************************************************

End of File

*/

 