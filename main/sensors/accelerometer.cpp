#include <stdio.h>
#include "sensor/Accelerometer.h"
#include "interface/i2c.h"

Accelerometer::Accelerometer(I2C* i2c_, uint8_t address_) :
    fast_mode(false),
    address(address_),
    i2c(i2c_) 
{ /* Empty constructor budy */ }

/**
 * @brief Read sensor data
 * @param acc_measurement Buffer to read the data to
 * @return 0 on success, otherwise error status
 */
int Accelerometer::read(acc_measurements& buffer)
{
    if (fetch_data() < ACCEL_XYZ_BUFFER) {
        return ACCEL_ERROR_STATUS;
    }
    
    // Save data to buffer
    if (fast_mode) {
        buffer.x = ((int16_t)(buffer_short[0] << 8) >> 2) * ACCEL_SCALE_FACTOR;
        buffer.y = ((int16_t)(buffer_short[1] << 8) >> 2)* ACCEL_SCALE_FACTOR;
        buffer.z = ((int16_t)(buffer_short[2] << 8) >> 2) * ACCEL_SCALE_FACTOR;
    }
    else {
        buffer.x = ((int16_t)((buffer_full[0] << 8) | buffer_full[1]) >> 2) * ACCEL_SCALE_FACTOR;
        buffer.y = ((int16_t)((buffer_full[2] << 8) | buffer_full[3]) >> 2) * ACCEL_SCALE_FACTOR;
        buffer.z = ((int16_t)((buffer_full[4] << 8) | buffer_full[5]) >> 2) * ACCEL_SCALE_FACTOR;
    }

    return 0;
}

/**
 * @brief Enable accelerometer in fast mode
 * Reading will be decreased to 8 bits, therefore resolution decreases
 * @return 0 on success, error status on error
 */
int Accelerometer::enable_in_fast_mode(const uint& timeout = 0)
{   
    disable();

    // Write new configuration
    uint8_t config = 0b00001111;
    esp_err_t result = i2c->write_register(address, CTRL_REG1, &config, 1, timeout);
    if (result == ESP_OK) fast_mode = true;

    vTaskDelay(10);

    return 0;
}

/**
 * @brief Enable accelerometer in normal mode
 * @return ESP operation status code
 */
int Accelerometer::enable(const uint& timeout = 0)
{
    disable(timeout);

    uint8_t config = 0b00001101;
    esp_err_t result = i2c->write_register(address, CTRL_REG1, &config, 1, timeout);
    if (result == ESP_OK) fast_mode = false;
    
    vTaskDelay(10);

    return result;
}

/**
 * @brief Disable accelerometer
 * @param timeout Timeout for the transaction 
 * @return ESP operation status code
 */
esp_err_t Accelerometer::disable(const uint& timeout = 0)
{   
    esp_err_t result = i2c->write_register(address, ACCEL_CTRL_REG1, &config, 1, timeout);
    vTaskDelay(10);

    return result;
}

/**
 * @brief Fetch raw data from the device
 * @return ESP operation status code
 */
esp_err_t Accelerometer::fetch_data(const uint& timeout = 0)
{
    if (fast_mode) {
        return i2c->read_register(address, ACCEL_OUT_X_MSB,
                                  buffer_short, ACCEL_XYZ_BUFFER, 
                                  timeout);
    }
    else {
        return i2c->read_register(address, ACCEL_OUT_X_MSB,
                                  buffer_full, ACCEL_XYZ_BUFFER_FULL,
                                  timeout);
    }
}

/**
 * @brief Returns current orientation of the cube
 * @return Side index of the cube
 * On error, return ORIENT_UNKNOWN
 */
uint8_t Accelerometer::get_side()
{
    if (Accelerometer::read(internal_measurements) == ACCEL_ERROR_STATUS) {
        return ACCEL_ORIENT_UNKNOWN;
    };

    if      (internal_measurements.z > ACCEL_THRESHOLD)      return 0;
    else if (internal_measurements.y < -1 * ACCEL_THRESHOLD) return 1;
    else if (internal_measurements.x < -1 * ACCEL_THRESHOLD) return 2;
    else if (internal_measurements.x > ACCEL_THRESHOLD)      return 3;
    else if (internal_measurements.y > ACCEL_THRESHOLD)      return 4;
    else if (internal_measurements.z < -1 * ACCEL_THRESHOLD) return 5;
    else                                                     return ACCEL_ORIENT_UNKNOWN;
}