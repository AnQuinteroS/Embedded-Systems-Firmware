# I2C Sensor Monitor (ESP32)

## Technical Overview
This project implements a firmware module for the **ESP32** microcontroller to interface with environmental sensors using the **I2C (Inter-Integrated Circuit)** communication protocol. 

Unlike high-level libraries that hide the complexity of data acquisition, this implementation focuses on manual register addressing and raw data processing, demonstrating a deep understanding of how software interacts with hardware.

## Key Engineering Concepts
* **Bitwise Operations:** The script reads data in 8-bit chunks (MSB and LSB) and utilizes bitwise shifting (`<<`) and the OR operator (`|`) to reconstruct the 20-bit raw temperature value.
* **Communication Protocol:** Implements the I2C master-slave architecture, handling bus initialization (`Wire.begin()`), error checking for device presence, reading the factory calibration registers, and switching the sensor from sleep to normal mode.
* **Efficiency:** Uses `constexpr` for memory-safe constant definition, avoiding the overhead of `#define` and ensuring type safety at compile-time.

## How to Test
1. **Hardware:** ESP32 Development Board + I2C Sensor (e.g., BMP280, BME280).
2. **Environment:** Arduino IDE or PlatformIO.
3. **Execution:** Open the Serial Monitor at 115200 baud to observe real-time data polling and calibration.

## Relevance to AI Code Evaluation
Generative AI often struggles with hardware-specific nuances, such as correct register addresses or bitwise reconstruction logic. My expertise allows me to identify if a model is "hallucinating" I2C addresses or if it fails to account for memory constraints in embedded environments.