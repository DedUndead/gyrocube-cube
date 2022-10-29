#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "accelerometer.hpp"
#include "i2c.hpp"
#include "esp_log.h"

Accelerometer::Accelerometer(I2C* i2c_, uint8_t address_, const bool& fastmode_, const uint16_t& threshold) :
    i2c(i2c_),
    address(address_),
    fastmode(fastmode_),
    threshold(threshold)
{ /* Empty constructor body */ }

/**
 * @brief Read sensor data
 * @param buffer Buffer to read the data to
 * @param timeout Read operation timeout
 * @return ESP operation status code 
 */
esp_err_t Accelerometer::read(accel_measurements& buffer, const uint& timeout)
{   
    // Fetch raw data
    uint8_t* raw_buffer = buffer_full;
    uint8_t raw_buffer_len = ACCEL_XYZ_BUFFER_FULL_LEN;
    if (fastmode) {
        raw_buffer = buffer_short;
        raw_buffer_len = ACCEL_XYZ_BUFFER_LEN;
    }
    esp_err_t fetch_result = fetch_data(raw_buffer, raw_buffer_len, timeout);
    if (fetch_result != ESP_OK) return fetch_result;
    
    // Apply transfer function to raw data
    if (fastmode) {
        buffer.x = ((int16_t)(raw_buffer[0] << 8) >> 2) * ACCEL_SCALE_FACTOR;
        buffer.y = ((int16_t)(raw_buffer[1] << 8) >> 2) * ACCEL_SCALE_FACTOR;
        buffer.z = ((int16_t)(raw_buffer[2] << 8) >> 2) * ACCEL_SCALE_FACTOR;
    }
    else {
        buffer.x = ((int16_t)((raw_buffer[0] << 8) | raw_buffer[1]) >> 2) * ACCEL_SCALE_FACTOR;
        buffer.y = ((int16_t)((raw_buffer[2] << 8) | raw_buffer[3]) >> 2) * ACCEL_SCALE_FACTOR;
        buffer.z = ((int16_t)((raw_buffer[4] << 8) | raw_buffer[5]) >> 2) * ACCEL_SCALE_FACTOR;
    }

    return fetch_result;
}

/**
 * @brief Enable accelerometer in normal mode
 * @param timeout Write operation timeout
 * @return ESP operation status code 
 */
esp_err_t Accelerometer::enable(const uint& timeout) const
{
    disable(timeout);

    uint8_t config;
    if (fastmode) config = 0b00001111;
    else          config = 0b00001101;

    esp_err_t result = i2c->write_register(address, ACCEL_CTRL_REG1, &config, 1, timeout);

    return result;
}

/**
 * @brief Disable accelerometer
 * @param timeout Write operation timeout
 * @return ESP operation status code
 */
esp_err_t Accelerometer::disable(const uint& timeout) const
{   
    uint8_t config = 0b00001000;

    esp_err_t result = i2c->write_register(address, ACCEL_CTRL_REG1, &config, 1, timeout);

    return result;
}

/**
 * @brief Fetch raw data from the device
 * @param buffer Target buffer
 * @param buffer_len Length of the target buffer
 * @param timeout Read operation timeout
 * @return ESP operation status code
 */
esp_err_t Accelerometer::fetch_data(uint8_t* buffer, const uint8_t& buffer_len, const uint& timeout)
{
    return i2c->read_register(address, ACCEL_OUT_X_MSB, buffer, buffer_len, timeout);
}

/**
 * @brief Returns current orientation of the cube
 * @param timeout Read operation timeout
 * @return Side index of the cube
 * On error, return ORIENT_UNKNOWN
 */
uint8_t Accelerometer::get_side(const uint& timeout)
{   
    accel_measurements reading;
    if (this->read(reading, timeout) != ESP_OK) {
        return ACCEL_ORIENT_UNKNOWN;
    };
    ESP_LOGI("ACCEL", "x %d y %d z %d", reading.x, reading.y, reading.z);

    if      (reading.z > threshold)      return 0;
    else if (reading.y < -1 * threshold) return 1;
    else if (reading.x < -1 * threshold) return 2;
    else if (reading.x > threshold)      return 3;
    else if (reading.y > threshold)      return 4;
    else if (reading.z < -1 * threshold) return 5;
    else                                 return ACCEL_ORIENT_UNKNOWN;
}
