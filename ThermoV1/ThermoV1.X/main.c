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


#define FOSC 16000000UL
#define FCY  (FOSC / 2)
#define MAX_MEASUREMENTS 10
#include <libpic30.h>

#include "mcc_generated_files/system.h"

 uint8_t flag = 0;
 uint8_t temperature;
 uint16_t buffer;
 uint8_t bufferFull = 0;
 uint8_t historyIndex = 0; 
 uint16_t temperatureHistory[MAX_MEASUREMENTS];
 

/*
                         Main application
 */
 
 void DisplayMenu(void) {
     
     
    LCD_ClearScreen();
    unsigned int len = 16;
    char tempString[len];
    char tempString1[len];
   
    sprintf(tempString, " 1.Real-time\r\n ");
    sprintf(tempString1, " 2.View-History ");
    LCD_PutString(tempString, len);
    LCD_PutString(tempString1, len);
     __delay_ms(100);
    
}
 
 void StoreTemperature(uint16_t temperature) {
    // Store the temperature in the current index
    temperatureHistory[historyIndex] = temperature;
    // Increment the index and wrap around if needed
    historyIndex = (historyIndex + 1) % MAX_MEASUREMENTS;
}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
   
    
    
    unsigned int len = 16;
    char tempString[len];
    
      
    
    while (1)
    {
        DisplayMenu();
        
        
        if(PORTDbits.RD6 ==0){
            
            LCD_ClearScreen();
            sprintf(tempString, "%.1d C\r", temperature);
            LCD_PutString(tempString, len);
             __delay_ms(500)
        }
        
        if(PORTDbits.RD7 == 0){
            uint8_t measurements[MAX_MEASUREMENTS];
            FetchAllMeasurementsFromEEPROM(measurements);
            LCD_ClearScreen();
            for (uint8_t i = 0; i < MAX_MEASUREMENTS; i++) {
                uint8_t index = (historyIndex + i) % MAX_MEASUREMENTS; // Circular index
                if (measurements[index] != 0) { // Only display valid measurements
                    sprintf(tempString, "T%d: %.1f C\r\n", i + 1, measurements[index] / 10.0);
                    LCD_PutString(tempString, 16);
                    __delay_ms(500); // Allow time for display update
        }   
    }
        }
        
       
        if (flag){
            ADC1_ConversionResultBufferGet( &buffer);
            temperature = (((buffer * 3.3) / 1023)*100)-0.5;
            StoreTemperature(temperature * 10);
            
            if (historyIndex == 0) {
                SendAllMeasurementsToEEPROM(temperatureHistory);
        
           
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

