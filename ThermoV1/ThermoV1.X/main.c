/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.5
        Device            :  PIC24FJ128GB610
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


#define FOSC 8000000UL // FRC without PLL (mcc_generated_files/system.c)
#define FCY  (FOSC / 2)
#define MAX_MEASUREMENTS 10
#include <libpic30.h>
#include <time.h>

#include <stdio.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/rtcc.h"
#include "lcd.h"

 volatile uint8_t flag = 0;
 uint16_t temperature;                          // tenths of a degree C
 uint16_t buffer;
 uint8_t bufferFull = 0;
 uint8_t historyIndex = 0;
 struct tm currentTime;
 uint16_t temperatureHistory[MAX_MEASUREMENTS]; // tenths of a degree C
 uint8_t secondHistory[MAX_MEASUREMENTS];
 uint8_t minuteHistory[MAX_MEASUREMENTS];

#define NO_MEASUREMENT 0xFFFF // what an erased EEPROM reads

void SendAllMeasurementsToEEPROM(const uint16_t temperatures[], const uint8_t minutes[], const uint8_t seconds[]);
void FetchAllMeasurementsFromEEPROM(uint16_t temperatures[], uint8_t minutes[], uint8_t seconds[]);
 

/*
                         Main application
 */
 
 void DisplayMenu(void) {
     
     
    LCD_ClearScreen();
    unsigned int len = 16;
    char tempString[len + 1];  // 16 characters and the terminating NUL
    char tempString1[len + 1];
   
    sprintf(tempString, " 1.Real-time\r\n ");
    sprintf(tempString1, " 2.View-History ");
    LCD_PutString(tempString, len);
    LCD_PutString(tempString1, len);
    __delay_ms(100);
    
}
 
 void StoreTemperature(uint16_t temperature) {
    // Store the temperature in the current index
    temperatureHistory[historyIndex] = temperature;
    RTCC_TimeGet( &currentTime);
    minuteHistory[historyIndex] = currentTime.tm_min;
    secondHistory[historyIndex] = currentTime.tm_sec;
    // Increment the index and wrap around if needed
    historyIndex = (historyIndex + 1) % MAX_MEASUREMENTS;
}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
   
    
    
    unsigned int len = 16;
    char tempString[24];



    while (1)
    {
        DisplayMenu();
        
        
        if(PORTDbits.RD6 ==0){
            
            LCD_ClearScreen();
            sprintf(tempString, "%.1f C\r", temperature / 10.0);
            LCD_PutString(tempString, len);
             __delay_ms(500);
        }
        
        if(PORTDbits.RD7 == 0){
            uint16_t temperatures[MAX_MEASUREMENTS];
            uint8_t minutes[MAX_MEASUREMENTS];
            uint8_t seconds[MAX_MEASUREMENTS];
            FetchAllMeasurementsFromEEPROM(temperatures, minutes, seconds);
            LCD_ClearScreen();
            // The EEPROM holds the last full history, oldest first
            for (uint8_t i = 0; i < MAX_MEASUREMENTS; i++) {
                if (temperatures[i] != NO_MEASUREMENT) { // Only display valid measurements
                    // 16 characters, one LCD line: "T1   23.5C 12:34"
                    sprintf(tempString, "T%-2u %5.1fC %02u:%02u\r\n", i + 1, temperatures[i] / 10.0, minutes[i], seconds[i]);
                    LCD_PutString(tempString, sizeof tempString);
                    __delay_ms(500); // Allow time for display update
        }   
    }
        }
        
       
        if (flag){
            flag = 0;
            ADC1_ConversionResultBufferGet( &buffer);
            // Sensor: 500 mV at 0 C, 10 mV/C; 10-bit ADC, 3.3 V reference
            double celsius = (((buffer * 3.3) / 1023) - 0.5) * 100;
            temperature = (celsius > 0) ? celsius * 10 + 0.5 : 0; // tenths, unsigned
            StoreTemperature(temperature);
            
            if (historyIndex == 0) {
                SendAllMeasurementsToEEPROM(temperatureHistory, minuteHistory, secondHistory);
        
           
    }
        
        
    }
            
        }
        
      return 1;
    }


   

/**
 End of File
*/

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _ADC1Interrupt ( void )
{
    // clear the ADC interrupt flag
    flag = 1;
    IFS0bits.AD1IF = 0;
}

