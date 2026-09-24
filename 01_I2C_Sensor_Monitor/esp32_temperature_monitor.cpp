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
constexpr uint8_t CALIB_REGISTER_START = 0x88;  // dig_T1..dig_T3 (0x88 to 0x8D)
constexpr uint8_t CTRL_MEAS_REGISTER = 0xF4;
constexpr uint8_t CTRL_MEAS_NORMAL_MODE = 0x27; // osrs_t x1, osrs_p x1, normal mode

// Factory calibration coefficients. Each sensor has its own values, stored in
// registers 0x88 to 0x8D (little-endian). They are read from the chip in setup().
uint16_t dig_T1 = 0;
int16_t dig_T2 = 0;
int16_t dig_T3 = 0;

// Reads the three temperature calibration words from the sensor.
bool read_calibration() {
    Wire.beginTransmission(SENSOR_I2C_ADDRESS);
    Wire.write(CALIB_REGISTER_START);
    if (Wire.endTransmission() != 0) return false;

    Wire.requestFrom((uint8_t)SENSOR_I2C_ADDRESS, (uint8_t)6);
    if (Wire.available() != 6) return false;

    // Separate read calls guarantee the LSB is read before the MSB
    uint8_t t1_lsb = Wire.read();
    uint8_t t1_msb = Wire.read();
    uint8_t t2_lsb = Wire.read();
    uint8_t t2_msb = Wire.read();
    uint8_t t3_lsb = Wire.read();
    uint8_t t3_msb = Wire.read();

    dig_T1 = (uint16_t)((t1_msb << 8) | t1_lsb);
    dig_T2 = (int16_t)((t2_msb << 8) | t2_lsb);
    dig_T3 = (int16_t)((t3_msb << 8) | t3_lsb);
    return true;
}

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

    if (!read_calibration()) {
        Serial.println("CRITICAL ERROR: Could not read calibration data.");
        while (true);
    }

    // The BMP280 starts in sleep mode: wake it up so it measures continuously.
    // Without this, the data registers keep their reset value (0x80000).
    Wire.beginTransmission(SENSOR_I2C_ADDRESS);
    Wire.write(CTRL_MEAS_REGISTER);
    Wire.write(CTRL_MEAS_NORMAL_MODE);
    Wire.endTransmission();
    delay(100); // Wait for the first conversion
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
