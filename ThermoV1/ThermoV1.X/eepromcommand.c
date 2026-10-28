

#include "mcc_generated_files/mcc.h"
#include <time.h>
#define FOSC 8000000UL // FRC without PLL (mcc_generated_files/system.c)
#define FCY  (FOSC / 2)

#include <libpic30.h>

// EEPROM Instructions
#define EEPROM_READ  0x03
#define EEPROM_WRITE 0x02
#define EEPROM_WREN  0x06
#define EEPROM_RDSR  0x05
#define EEPROM_WIP   0x01  // Status register: write in progress


#define TEMPERATURE_SIZE 2 // tenths of a degree C, high byte first
#define TIME_SIZE 1
#define MAX_MEASUREMENTS 10
#define EEPROM_START_ADDRESS 0x0010  // temperatures
#define EEPROM_START_ADDRESS1 0x0100 // minutes
#define EEPROM_START_ADDRESS2 0x1000 // seconds

// Control Chip Select
#define CS_SetLow()  LATGbits.LATG0 = 0
#define CS_SetHigh() LATGbits.LATG0 = 1



uint8_t EEPROM_ReadStatus(void) {
    uint8_t status;

    CS_SetLow();
    SPI1_Exchange8bit(EEPROM_RDSR);
    status = SPI1_Exchange8bit(0xFF);
    CS_SetHigh();

    return status;
}

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

    // The write takes up to 5 ms, and the EEPROM ignores commands until it ends
    while (EEPROM_ReadStatus() & EEPROM_WIP);
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

void SendAllMeasurementsToEEPROM(const uint16_t temperatures[], const uint8_t minutes[], const uint8_t seconds[]) {
    uint16_t address = EEPROM_START_ADDRESS;
    uint16_t address1 = EEPROM_START_ADDRESS1;
    uint16_t address2 = EEPROM_START_ADDRESS2;

    for (uint8_t i = 0; i < MAX_MEASUREMENTS; i++) {
        EEPROM_Write(address, temperatures[i] >> 8);  // Write each measurement to EEPROM
        EEPROM_Write(address + 1, temperatures[i] & 0xFF);
        EEPROM_Write(address1, minutes[i]);
        EEPROM_Write(address2, seconds[i]);
        address += TEMPERATURE_SIZE;
        address1 += TIME_SIZE;
        address2 += TIME_SIZE;
    }
}

void FetchAllMeasurementsFromEEPROM(uint16_t temperatures[], uint8_t minutes[], uint8_t seconds[]) {
    uint16_t address = EEPROM_START_ADDRESS;
    uint16_t address1 = EEPROM_START_ADDRESS1;
    uint16_t address2 = EEPROM_START_ADDRESS2;

    for (uint8_t i = 0; i < MAX_MEASUREMENTS; i++) {
        temperatures[i] = ((uint16_t)EEPROM_Read(address) << 8) | EEPROM_Read(address + 1);  // Read each measurement from EEPROM
        minutes[i] = EEPROM_Read(address1);
        seconds[i] = EEPROM_Read(address2);
        address += TEMPERATURE_SIZE;
        address1 += TIME_SIZE;
        address2 += TIME_SIZE;            // Move to the next memory location
    }
}
