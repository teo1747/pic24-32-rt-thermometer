

#include "mcc_generated_files/mcc.h"
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

void SendAllMeasurementsToEEPROM(const uint8_t measurements[]) {
    uint16_t address = EEPROM_START_ADDRESS;

    for (uint8_t i = 0; i < MAX_MEASUREMENTS; i++) {
        EEPROM_Write(address, measurements[i]);  // Write each measurement to EEPROM
        address += MEASUREMENT_SIZE;            // Move to the next memory location
        __delay_ms(5);  // Wait for the write cycle to complete
    }
}

void FetchAllMeasurementsFromEEPROM(uint8_t measurements[]) {
    uint16_t address = EEPROM_START_ADDRESS;

    for (uint8_t i = 0; i < MAX_MEASUREMENTS; i++) {
        measurements[i] = EEPROM_Read(address);  // Read each measurement from EEPROM
        address += MEASUREMENT_SIZE;            // Move to the next memory location
        __delay_ms(5);
    }
}
