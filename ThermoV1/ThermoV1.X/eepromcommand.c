

#include "mcc_generated_files/mcc.h"
#include <time.h>
#define FOSC 16000000UL
#define FCY  (FOSC / 2)

#include <libpic30.h>

// EEPROM Instructions
#define EEPROM_READ  0x03
#define EEPROM_WRITE 0x02
#define EEPROM_WREN  0x06


#define MEASUREMENT_SIZE 1 
#define MAX_MEASUREMENTS 10
#define EEPROM_START_ADDRESS 0x0010
#define EEPROM_START_ADDRESS1 0x0100
#define EEPROM_START_ADDRESS2 0x1000

// Control Chip Select
#define CS_SetLow()  LATGbits.LATG0 = 0
#define CS_SetHigh() LATGbits.LATG0 = 1



void EEPROM_Write(uint16_t address, uint8_t data) {
    // Enable write operations
    CS_SetLow();
    SPI1_Exchange8bit(EEPROM_WREN);
    CS_SetHigh();

    // Write data
    CS_SetLow();
    SPI1_Exchange8bit(EEPROM_WRITE);               // Write command
    SPI1_Exchange8bit((address >> 8) & 0xFF);     // High address byte
    SPI1_Exchange8bit(address & 0xFF);            // Low address byte
    SPI1_Exchange8bit(data);                      // Data byte
    CS_SetHigh();
}

uint8_t EEPROM_Read(uint16_t address) {
    uint8_t data;

    // Read data
    CS_SetLow();
    SPI1_Exchange8bit(EEPROM_READ);               // Read command
    SPI1_Exchange8bit((address >> 8) & 0xFF);     // High address byte
    SPI1_Exchange8bit(address & 0xFF);            // Low address byte
    data = SPI1_Exchange8bit(0xFF);               // Dummy data for read
    CS_SetHigh();

    return data;
}

void SendAllMeasurementsToEEPROM(const uint8_t measurements[],const uint8_t measurements1[],const uint8_t measurements2[]) {
    uint16_t address = EEPROM_START_ADDRESS;
    uint16_t address1 = EEPROM_START_ADDRESS1;
    uint16_t address2 = EEPROM_START_ADDRESS2;

    for (uint8_t i = 0; i < MAX_MEASUREMENTS; i++) {
        EEPROM_Write(address, measurements[i]);  // Write each measurement to EEPROM
        EEPROM_Write(address1, measurements1[i]);
        EEPROM_Write(address2, measurements2[i]);
        address += MEASUREMENT_SIZE;
        address1 += MEASUREMENT_SIZE; 
        address2 += MEASUREMENT_SIZE; 
        __delay_ms(5);  // Wait for the write cycle to complete
    }
      
   
}

void FetchAllMeasurementsFromEEPROM(uint8_t measurements[],uint8_t measurements1[],uint8_t measurements2[]) {
    uint16_t address = EEPROM_START_ADDRESS;
    uint16_t address1 = EEPROM_START_ADDRESS1;
    uint16_t address2 = EEPROM_START_ADDRESS2;

    for (uint8_t i = 0; i < MAX_MEASUREMENTS; i++) {
        measurements[i] = EEPROM_Read(address);  // Read each measurement from EEPROM
        measurements1[i] = EEPROM_Read(address1);
        measurements2[i] = EEPROM_Read(address2);
        address += MEASUREMENT_SIZE;
        address1 += MEASUREMENT_SIZE; 
        address2 += MEASUREMENT_SIZE;            // Move to the next memory location
        __delay_ms(5);
    }
}
