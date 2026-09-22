                AquaMesh
                   │
        ┌──────────┴──────────┐
        │                     │
     Network              Firmware
     Services              Update
        │                     │
        ▼                     ▼
 ESP32 Communication ◄── Bootloader Bridge
        │
    Serial / JSON
        │
        ▼
 davinciarduino2025
 STM32 Machine Controller
        │
        ▼
 Pumps / Valves / CO₂ / Compressor
 Hot Tank / Agitator / Flow / NTC / LEDs
