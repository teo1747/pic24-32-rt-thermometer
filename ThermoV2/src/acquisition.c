/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    acquisition.c

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

#include "acquisition.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/peripheral/adchs/plib_adchs.h"
#include <stdio.h>

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
    This structure should be initialized by the ACQUISITION_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

ACQUISITION_DATA acquisitionData;

 QueueHandle_t  myQueue;
 SemaphoreHandle_t  mySemaphore;

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


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void ACQUISITION_Initialize ( void )

  Remarks:
    See prototype in acquisition.h.
 */

void ACQUISITION_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    acquisitionData.state = ACQUISITION_STATE_INIT;
    ADCHS_Initialize();

    myQueue = xQueueCreate( 10, sizeof( uint16_t ) );
    mySemaphore = xSemaphoreCreateBinary();
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void ACQUISITION_Tasks ( void )

  Remarks:
    See prototype in acquisition.h.
 */

void ACQUISITION_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( acquisitionData.state )
    {
        /* Application's initial state. */
        case ACQUISITION_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                acquisitionData.state = ACQUISITION_STATE_SERVICE_TASKS;
            }
            break;
        }

        case ACQUISITION_STATE_SERVICE_TASKS:
        {
            ADCHS_GlobalEdgeConversionStart();
            if (ADCHS_ChannelResultIsReady(ADCHS_CH4))
            {  
                uint16_t temp_value = ADCHS_ChannelResultGet(ADCHS_CH4);
                if (myQueue == 0)
                {
                    break;
                }
                if(  xQueueSend( myQueue, &temp_value, 10) != 0)
                {
                    xSemaphoreGive(mySemaphore);
                    Led6_Toggle() ;
                }
            }
           

            
            break;
        }

        /* TODO: implement your application state machine.*/


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
