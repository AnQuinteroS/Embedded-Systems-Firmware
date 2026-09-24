/**
 * ESP32 I2C Temperature Sensor Monitor (BMP280)
 * 
 * Objective: Read raw 20-bit sensor data via I2C protocol and process it 
 * into human-readable temperature applying the required factory calibration.
 */
#include <Wire.h>
#include <Arduino.h>

constexpr uint8_t SENSOR_I2C_ADDRESS = 0x76;
constexpr uint8_t TEMP_REGISTER_MSB = 0xFA;

// BMP280 requires reading factory calibration registers (0x88 to 0x8D) for accurate data.
// In a production environment, these must be read via I2C during setup().
// These are placeholder values representing typical factory calibration coefficients.
uint16_t dig_T1 = 27504; 
int16_t dig_T2 = 26435;
int16_t dig_T3 = -1000;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Serial.println("System Initialized. Booting I2C Sensor...");

    Wire.beginTransmission(SENSOR_I2C_ADDRESS);
    if (Wire.endTransmission() != 0) {
        Serial.println("CRITICAL ERROR: Sensor not found on I2C bus.");
        while (true); 
    }
    Serial.println("Sensor successfully detected.");
}

// Standard Bosch BMP280 compensation formula to convert 20-bit raw data to Celsius
int32_t t_fine;
float compensate_temperature(int32_t adc_T) {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T / 100.0f;
}

void loop() {
    Wire.beginTransmission(SENSOR_I2C_ADDRESS);
    Wire.write(TEMP_REGISTER_MSB);
    Wire.endTransmission();
    
    // BMP280 temperature data is 20 bits, spread across 3 bytes
    Wire.requestFrom((uint8_t)SENSOR_I2C_ADDRESS, (uint8_t)3);

    if (Wire.available() == 3) {
        // Separate read calls guarantee execution order in C++
        uint8_t msb = Wire.read();
        uint8_t lsb = Wire.read();
        uint8_t xlsb = Wire.read();

        // Reconstruct the 20-bit raw value
        int32_t raw_temperature = (msb << 12) | (lsb << 4) | (xlsb >> 4);

        // Apply factory calibration
        float actual_temperature = compensate_temperature(raw_temperature);

        Serial.print("Current Temperature: ");
        Serial.print(actual_temperature);
        Serial.println(" °C");
    } else {
        Serial.println("Warning: Data request failed or incomplete payload received.");
    }

    delay(2000);
}
