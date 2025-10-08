# FreeRTOS Sensor and CAN simulation 

This project simulates a simple sensor system using FreeRTOS on a teensy4.1 board. It demonstrates basic multi-tasking and queue based inter task communication. 

## Features

- **Sensor Task**: Generates a virtual signal using a random number generator, applies a moving average filter, and detects outliers. 
- **Can Task**: Packages filtered sensor data into a CAN frame and prints it to Serial. 

## Can Frame Format

| Field | Bytes | Description |
|-------|-------|-------------|
| CAN ID | — | 0x123 |
| Byte 0–1 | 2 | Filtered sensor value (0–100%) |
| Byte 2 | 1 | Error flag (0 = OK, 1 = ERROR) |

## Scaling and Error Flag 

- Filtered value is a percentage (0 - 100%).
- Error Flag is set to 1 when value <0% or >100%, otherwise 0.

## How to Compile and Run

1. Open project in VScode with PlatformIO extension installed.
2. Connect your board to the computer.
3. Build using the check mark and then upload with the arrow button.
4. Open Serial Monitor and observe the CAN messages.
