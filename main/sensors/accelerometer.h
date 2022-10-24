#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H


#include "pico/stdio.h"
#include "interfaces/i2c.h"

#define ACCEL_DEF_ADDRESS      0x1E
#define ACCEL_CTRL_REG1        0x2A
#define ACCEL_OUT_X_MSB        0x01
#define ACCEL_SCALE_FACTOR     (float)0.244

#define ACCEL_XYZ_BUFFER_LEN       3
#define ACCEL_XYZ_BUFFER_FULL_LEN  6
#define ACCEL_ERROR_STATUS         0xEFFF
#define ACCEL_ORIENT_UNKNOWN       0x06
#define ACCEL_THRESHOLD            850

typedef struct accel_measurements {
    int16_t x;
    int16_t y;
    int16_t z;
} accel_measurements;

/**
 * @brief Abstraction of accelerometer class
 * Provides interface to read/config values on chip
 */
class Accelerometer {
public:
    Accelerometer(I2C* i2c_,
                  uint8_t address_ = DEF_ADDRESS,
                  const bool& fastmode_ = false,
                  const uint16_t& threshold = ACCEL_THRESHOLD);
    esp_err_t read(acc_measurements& buffer, const uint& timeout = 0) const;
    esp_err_t enable(const uint& timeout = 0) const;
    esp_err_t disable(const uint& timeout = 0) const;
    uint8_t get_side(const uint& timeout = 0) const;
private:
    esp_err_t fetch_data(uint8_t* buffer, const uint8_t& buffer_len, uint& timeout = 0);

    bool fast_mode;                             // Flag determines fast mode operation
    uint8_t buffer_short[ACCEL_XYZ_BUFFER];     // Short buffer for fast reading
    uint8_t buffer_full[ACCEL_XYZ_BUFFER_FULL]; // Full buffer for full reading

    uint8_t address;
    I2C* i2c;
};


#endif