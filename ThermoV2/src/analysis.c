/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    analysis.c

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

#include "analysis.h"
#include "acquisition.h"
#include "config/default/peripheral/gpio/plib_gpio.h"

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
    This structure should be initialized by the ANALYSIS_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

ANALYSIS_DATA analysisData;

QueueHandle_t  myQueue1;
SemaphoreHandle_t  mySemaphore1;

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
    void ANALYSIS_Initialize ( void )

  Remarks:
    See prototype in analysis.h.
 */

void ANALYSIS_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    analysisData.state = ANALYSIS_STATE_INIT;

    myQueue1 = xQueueCreate( 10, sizeof( double ) );
    mySemaphore1 = xSemaphoreCreateBinary();

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void ANALYSIS_Tasks ( void )

  Remarks:
    See prototype in analysis.h.
 */

void ANALYSIS_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( analysisData.state )
    {
        /* Application's initial state. */
        case ANALYSIS_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                analysisData.state = ANALYSIS_STATE_SERVICE_TASKS;
            }
            break;
        }

        case ANALYSIS_STATE_SERVICE_TASKS:
        {
            uint16_t  temp_val_A;// recupere la valeur de son frere temp_value
            double  Temp_real_value;
             if (xSemaphoreTake(mySemaphore, 0) != 0)
             {
                  if (myQueue != NULL )
                    {
                        if (xQueueReceive( myQueue, &temp_val_A, 0) != 0)
                        {
                             Temp_real_value = (((temp_val_A*3.3)/4095) - 0.5)*100;
                              if(xQueueSend( myQueue1, &Temp_real_value, 10) != 0)
                              {
                                  xSemaphoreGive(mySemaphore1);
                                  Led7_Toggle();
                              }
                        }
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
