/*
 * File:   temp_message.h
 *
 * The message the analysis task sends to the display task through
 * myQueueTemp: one measurement in both units, the unit to show, and when it
 * was taken.
 */

#ifndef _TEMP_MESSAGE_H
#define _TEMP_MESSAGE_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>

typedef enum
{
    TEMP_UNIT_CELSIUS = 0,
    TEMP_UNIT_FAHRENHEIT
} TempUnit_t;

/* Temperature message structure (received from queue) */
typedef struct
{
    int16_t value_c;        // temperature value
    int16_t value_f;        // temperature value in Fahrenheit
    TempUnit_t unit;        // unit of temperature
    uint32_t timestamp;     // Time in seconds since boot
} TempMessage_t;

/* The unit the analysis task puts in each new message; the button changes it */
extern volatile TempUnit_t currentTempUnit;

TempUnit_t ToggleTempUnit(TempUnit_t unit);

#endif /* _TEMP_MESSAGE_H */
