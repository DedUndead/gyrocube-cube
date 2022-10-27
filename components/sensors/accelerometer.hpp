#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H


#include "i2c.hpp"
#include "freertos/FreeRTOS.h"

#define ACCEL_DEFAULT_ADDRESS   (0x1E)
#define ACCEL_CTRL_REG1         0x2A
#define ACCEL_OUT_X_MSB         0x01
#define ACCEL_SCALE_FACTOR      (float)0.244

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
                  uint8_t address_ = ACCEL_DEFAULT_ADDRESS,
                  const bool& fastmode_ = true,
                  const uint16_t& threshold = ACCEL_THRESHOLD);
    esp_err_t read(accel_measurements& buffer, const uint& timeout = portMAX_DELAY);
    esp_err_t enable(const uint& timeout = portMAX_DELAY) const;
    esp_err_t disable(const uint& timeout = portMAX_DELAY) const;
    uint8_t get_side(const uint& timeout = portMAX_DELAY);
private:
    esp_err_t fetch_data(uint8_t* buffer, const uint8_t& buffer_len, const uint& timeout = portMAX_DELAY);

    I2C* i2c;
    const uint8_t address;
    const bool fastmode; 
    const uint threshold;

    uint8_t buffer_short[ACCEL_XYZ_BUFFER_LEN];     // Short buffer for fast reading
    uint8_t buffer_full[ACCEL_XYZ_BUFFER_FULL_LEN]; // Full buffer for full reading
};


#endif