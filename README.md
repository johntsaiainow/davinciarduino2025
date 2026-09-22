# DaVinci Arduino 2025

Main controller firmware for the **DrinkStation DaVinci 20 Plus** beverage dispensing system.

This project implements the real-time machine-control layer responsible for beverage dispensing, temperature management, pumps, valves, compressor control, hot-water operation, flow measurement, safety functions, user controls, and communication with external IoT modules.

## Overview

The firmware runs on an **STM32F103** microcontroller using the Arduino framework and PlatformIO.

It serves as the primary embedded controller for the dispenser.

```text
                    IoT / Communication Module
                             │
                         Serial / JSON
                             │
                             ▼
                  ┌──────────────────────┐
                  │      STM32F103       │
                  │                      │
                  │  DaVinci Controller  │
                  └──────────┬───────────┘
                             │
          ┌──────────────────┼──────────────────┐
          │                  │                  │
          ▼                  ▼                  ▼
     Dispensing          Thermal            Machine
       System            Control            Interface
          │                  │                  │
     ┌────┴────┐       ┌─────┴─────┐      ┌────┴────┐
     │ Valves  │       │Compressor │      │ Buttons │
     │ Pumps   │       │ Hot Tank  │      │  LEDs   │
     │ CO₂     │       │ Agitator  │      │ Safety  │
     │ Flow    │       │   NTCs    │      │ Levels  │
     └─────────┘       └───────────┘      └─────────┘
```

## Core Functions

The firmware provides control for:

* Sparkling water dispensing
* Cold water dispensing
* Alkaline water dispensing
* Hot water dispensing
* Automatic volume dispensing
* Flow measurement
* Water pump control
* CO₂ valve control
* Main and dispensing valves
* Compressor control
* Chiller bucket management
* Hot tank temperature control
* Agitator pump control
* Filter status tracking
* Child-safety handling
* Push-button and LED interfaces
* Temperature monitoring
* EEPROM-based configuration
* Machine status reporting
* Serial / JSON communication

## Operating Modes

The controller supports multiple machine operating modes, including:

```text
Manual
Autofill
Calibration
Configuration
Chiller Bucket Refill
Hot Tank Configuration
Chiller Bucket Configuration
Safety
Hot Water Dispense
```

The firmware manages transitions between these modes according to user input, machine state, sensor conditions, and remote commands.

## Hardware Platform

The current PlatformIO target is:

**STM32F103RC**

using the Arduino STM32 framework.

Programming and debugging are configured for **ST-Link**.

```text
MCU        STM32F103
Framework  Arduino
Build      PlatformIO
Programmer ST-Link
Serial     115200 baud
```

USB CDC is also enabled for development and diagnostic communication.

## Architecture

The firmware is divided into functional control modules.

```text
src/
├── main.cpp
├── main.h
│
├── CompressorControl.cpp
├── CompressorControl.h
│
├── HotTankControl.cpp
├── HotTankControl.h
│
├── AgitatorPumpControl.cpp
├── AgitatorPumpControl.h
│
├── FilterStatusCheck.cpp
├── FilterStatusCheck.h
│
├── DateTime.cpp
├── DateTime.h
│
├── temp_read.cpp
├── temp_read.h
│
├── Button.h
├── button.cpp
│
├── Led.h
├── led.cpp
│
└── AppTypes.h
```

Each subsystem handles a specific part of the machine while `main.cpp` coordinates overall operation and system state.

## Communication

The controller communicates with external systems over serial at:

```text
115200 baud
```

Machine information and events can be exchanged using structured JSON messages.

This interface allows an external communication module to provide higher-level services such as:

* Wi-Fi connectivity
* MQTT
* Remote monitoring
* IoT integration
* Diagnostics
* Firmware management
* Cloud or local network services

The STM32 remains responsible for real-time machine operation while network functionality can remain isolated in a separate communication layer.

## Building

This project uses **PlatformIO**.

Clone the repository:

```bash
git clone <repository-url>
cd davinciarduino2025
```

Build the firmware:

```bash
pio run
```

Upload using ST-Link:

```bash
pio run -t upload
```

Monitor serial output:

```bash
pio device monitor
```

## Project Structure

```text
davinciarduino2025/
├── src/                    # Main controller firmware
├── include/                # Project include files
├── lib/                    # Local libraries
├── test/                   # Test resources
├── platformio.ini          # PlatformIO configuration
├── getMachineStatus.py     # Diagnostic utility
├── getMachineStatusTB.py
├── getNTC.py
├── getNTC2.py
├── setDateTime.py
├── gui.py
└── README.md
```

## Supporting Libraries

The project uses several embedded libraries, including:

* ArduinoJson
* ShiftRegister74HC595
* FlowMeter
* ErriezDS1302

Some libraries are included locally under `lib/`.

## Related AquaMesh Architecture

The DaVinci controller can operate as the real-time machine layer within a broader AquaMesh architecture.

```text
                 Network / Services
                        │
                        ▼
              ┌────────────────────┐
              │ ESP32 Communication│
              │       Module       │
              └─────────┬──────────┘
                        │
                   Serial / JSON
                        │
                        ▼
              ┌────────────────────┐
              │    STM32F103       │
              │ DaVinci Controller │
              └─────────┬──────────┘
                        │
                        ▼
               Physical Dispenser
```

This separation keeps network and service functions independent from time-sensitive machine control.

## Documentation

Detailed technical documentation should be maintained under `docs/`.

```text
docs/
├── architecture.md
├── hardware.md
├── pinout.md
├── operating-modes.md
├── dispensing.md
├── thermal-control.md
├── serial-protocol.md
├── commands.md
├── configuration.md
└── development.md
```

The root README intentionally provides only a high-level project overview.

## Development Utilities

Several Python utilities are included for development and diagnostics.

They provide functions such as:

* Reading machine status
* Reading NTC temperature information
* Setting controller date and time
* Testing serial communication
* Development GUI interaction

These tools are intended primarily for engineering, testing, and service use.

## Design Philosophy

### Real-Time Control First

Machine safety and physical control remain on the STM32 rather than depending on network availability.

### Separation of Responsibilities

The STM32 controls the machine.

External communication modules provide networking and higher-level services.

### Serviceability

Machine state, configuration, diagnostics, and control functions are accessible through a structured communication interface.

### Modular Control

Major machine functions are separated into dedicated controller modules so the firmware can evolve without turning the main control loop into a monolithic implementation.

## Status

This repository contains the current development firmware for the DaVinci 20 Plus controller.

The firmware continues to evolve as machine-control logic, IoT integration, diagnostics, and AquaMesh functionality are developed.

## License

See the repository license for licensing information.

---

**DaVinci 20 Plus Controller Firmware**

*Real-time machine control at the heart of the AquaMesh dispenser platform.*

