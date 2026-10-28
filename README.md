# PIC24/32 Real-Time Thermometer

A digital thermometer for Microchip PIC microcontrollers, built in three stages for a real-time operating systems lab (TP RT-Therm). It starts as a bare-metal PIC24 program and ends as a PIC32MZ application running on FreeRTOS with MPLAB Harmony v3.

| Version | MCU | Toolchain | Approach |
|---|---|---|---|
| [ThermoV1](ThermoV1/) | PIC24FJ1024GB610 | XC16 v2.10, MCC | Bare-metal super-loop, ADC interrupt, SPI EEPROM history |
| [ThermoV1.1](ThermoV1.1/) | PIC32MZ2048EFH100 | XC32 v4.60, Harmony v3 | First FreeRTOS port, three tasks |
| [ThermoV2](ThermoV2/) | PIC32MZ2048EFH100 | XC32 v4.60, Harmony v3 | FreeRTOS pipeline with queues and semaphores |

All projects were built with MPLAB X IDE v6 and flashed with the on-board PICkit (PKOB).

## ThermoV1: bare-metal PIC24

- ADC1 samples the temperature sensor (AN4) continuously. Its interrupt sets a flag, and the main loop reads the sample and converts it to tenths of a degree.
- A 16-character LCD shows a menu: **1. Real-time** (button on RD6) or **2. View history** (button on RD7).
- The last 10 readings are kept in a circular buffer, each with an RTCC timestamp (minutes:seconds). When the buffer wraps, the readings and timestamps are written to an external 25LC256 SPI EEPROM (`eepromcommand.c`, chip select on RG0). Each temperature takes 2 bytes, and each write waits for the EEPROM to finish.
- The history screen reads them back, oldest first, as `T1   24.8C 30:04` (reading number, °C, mm:ss).

## ThermoV2: FreeRTOS on PIC32MZ

The application is split into three Harmony tasks. Each stage passes its result to the next through a queue and signals it with a binary semaphore. Each stage also toggles an LED, so the pipeline can be watched on the board.

```mermaid
flowchart LR
  A["ACQUISITION_Tasks<br/>ADCHS channel 4, 12-bit<br/>toggles LED6"] -- "myQueue (uint16_t)<br/>+ mySemaphore" --> B["ANALYSIS_Tasks<br/>raw value to °C<br/>toggles LED7"]
  B -- "myQueue1 (double)<br/>+ mySemaphore1" --> C["DISPLAY_Tasks<br/>LCD on the PMP bus<br/>toggles LED9"]
```

The conversion assumes an analog sensor with a 500 mV offset and 10 mV/°C, on a 3.3 V reference:

```
T(°C) = (raw × 3.3 / 4095 − 0.5) × 100
```

ThermoV1 uses the same formula with its 10-bit ADC, dividing by 1023 instead of 4095.

FreeRTOS settings (`ThermoV2/src/config/default/FreeRTOSConfig.h`): preemptive scheduler, 1 kHz tick, `heap_1` with a 28 000-byte heap, three tasks at priority 1 with 1024-word stacks.

ThermoV1.1 is the first version of this pipeline. There, the display task starts the ADC conversion and reads its queue without waiting on a semaphore.

## Repository layout

```
ThermoV1/ThermoV1.X/         MPLAB X project, application code, PIC24 LCD driver, MCC drivers
ThermoV1/rapportPIC24MAMBO_MOTSOU_JUNIOR.pdf   PIC24 (V1) report
ThermoV1.1/src/              Application tasks and Harmony/FreeRTOS config
ThermoV1.1/Thermo_V1.X/      MPLAB X project
ThermoV2/src/                Application tasks and Harmony/FreeRTOS config
ThermoV2/Thermo.X/           MPLAB X project and PIC32 LCD driver (lcd.c, lcd.h)
TP_RT_Therm__Harmony_V3___FreeRTOS_.pdf   FreeRTOS lab report (in progress)
```

## Building

1. Install MPLAB X IDE v6, plus XC16 (for V1) or XC32 (for V1.1 and V2).
2. In MPLAB X, open the `.X` folder of the version you want.
3. Build and program the board.

To regenerate the drivers, open `Thermo.mc3` with MCC (V1) or the MCC Harmony content manager (V1.1 and V2).

## Notes

- **LCD drivers**: V1 has its own PIC24 driver (EPMP). V1.1 and V2 share the PIC32 driver in `ThermoV2/Thermo.X/` (PMP + Timer 2), which V1.1 links by relative path. The projects were first developed on Windows, where these links were absolute `D:/Microchip/...` paths.
## History

The commit history starts from [Soriane-Charle/Thermometre-temps-Reel](https://github.com/Soriane-Charle/Thermometre-temps-Reel).
