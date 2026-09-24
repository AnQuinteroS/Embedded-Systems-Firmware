/**
 * ESP32 I2C Temperature Sensor Monitor
 * * Objective: Read raw sensor data via I2C protocol, process the bitwise 
 * information into human-readable temperature, and transmit it over WiFi/Serial.
 * * Why this matters for AI/Software Engineering: 
 * Demonstrates hardware-software integration, bitwise operations, and 
 * handling systems with strict memory and processing constraints.
 */

#include <Wire.h> // Standard I2C library for ESP32/Arduino frameworks

// Define hardware constants (Using 'constexpr' for memory efficiency over '#define')
constexpr uint8_t SENSOR_I2C_ADDRESS = 0x76; 
constexpr uint8_t TEMP_REGISTER_MSB = 0xFA;
constexpr float TEMP_CALIBRATION_FACTOR = 0.01f;

void setup() {
    // Initialize serial communication at a high baud rate for faster debugging
    Serial.begin(115200);
    
    // Initialize the I2C bus
    Wire.begin();
    
    Serial.println("System Initialized. Booting I2C Sensor...");
    
    // Verify sensor connection
    Wire.beginTransmission(SENSOR_I2C_ADDRESS);
    if (Wire.endTransmission() != 0) {
        Serial.println("CRITICAL ERROR: Sensor not found on I2C bus.");
        while (true); // Halt execution to prevent undefined behavior
    }
    Serial.println("Sensor successfully detected.");
}

void loop() {
    // 1. Request 2 bytes of data from the sensor's temperature register
    Wire.beginTransmission(SENSOR_I2C_ADDRESS);
    Wire.write(TEMP_REGISTER_MSB);
    Wire.endTransmission();
    
    Wire.requestFrom(SENSOR_I2C_ADDRESS, (uint8_t)2);
    
    if (Wire.available() == 2) {
        // 2. Bitwise Operation: Combine Most Significant Byte (MSB) and Least Significant Byte (LSB)
        // We shift the MSB 8 bits to the left and use bitwise OR to append the LSB.
        uint16_t raw_temperature = (Wire.read() << 8) | Wire.read();
        
        // 3. Process the raw data using the calibration factor
        float actual_temperature = raw_temperature * TEMP_CALIBRATION_FACTOR;
        
        Serial.print("Current Temperature: ");
        Serial.print(actual_temperature);
        Serial.println(" °C");
    } else {
        Serial.println("Warning: Data request failed or incomplete payload received.");
    }
    
    // Delay to prevent flooding the serial monitor and save CPU cycles (simulating a polling rate)
    delay(2000); 
}