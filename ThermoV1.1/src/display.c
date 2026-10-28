/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    display.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "stdlib.h"
#include "stdio.h"
#include "display.h"
#include "analysis.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/peripheral/adchs/plib_adchs.h"
#include "../Thermo_V1.X/lcd.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the DISPLAY_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

DISPLAY_DATA displayData;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* Shown while S6 is not pressed */
static void DisplayMenu(void)
{
    /* Clear display */
    writeToLCD(CMDREG, 0x01);
    Delay_ms(2);

    writeString("RT-Therm V1");
    newLine();
    writeString("S6: temperature");
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void DISPLAY_Initialize ( void )

  Remarks:
    See prototype in display.h.
 */

void DISPLAY_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    displayData.state = DISPLAY_STATE_INIT;

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void DISPLAY_Tasks ( void )

  Remarks:
    See prototype in display.h.
 */

void DISPLAY_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( displayData.state )
    {
        /* Application's initial state. */
        case DISPLAY_STATE_INIT:
        {
            TMR2_Start();
            initializeLCD();
            bool appInitialized = true;


            if (appInitialized)
            {

                displayData.state = DISPLAY_STATE_SERVICE_TASKS;
            }
            break;
        }

        case DISPLAY_STATE_SERVICE_TASKS:
        {
            double temp;
            if( myQueue1 == 0 ){

                break;
            }
            if(xQueueReceive(myQueue1,&temp,0) == pdPASS){
                GPIO_PinToggle(GPIO_PIN_RA6);
                DisplayMenu();
                    if(GPIO_PinRead(GPIO_PIN_RD13) ==0){    // Active-low button S6

                        displayTemperature(temp);
                        Delay_ms(50);
                        break;

                    }

            }
            break;
        }

        /* TODO: implement your application state .*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
