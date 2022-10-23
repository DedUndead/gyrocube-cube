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
int Accelerometer::enable_in_fast_mode()
{   
    disable();

    // Write new configuration
    uint8_t config = 0b00001111;
    if (i2c->write(address, CTRL_REG1, &config, 1) == PICO_ERROR_GENERIC) {
        return ACC_ERROR_STATUS;
    }
    fast_mode = true;

    sleep_ms(10);

    return 0;
}

/**
 * @brief Enable accelerometer in normal mode
 * @return 0 on success, error status on error
 */
int Accelerometer::enable()
{
    disable();

    uint8_t config = 0b00001101;
    if (i2c->write(address, CTRL_REG1, &config, 1) == PICO_ERROR_GENERIC) {
        return ACC_ERROR_STATUS;
    }
    fast_mode = false;
    
    sleep_ms(10);

    return 0;
}

/**
 * @brief Disable accelerometer
 * @return 0 on success, error status on error
 */
int Accelerometer::disable()
{
    // Set control register to read values in Xmsb, Ymsb, Zmsb order
    uint8_t config = 0b00001000;
    if (i2c->write(address, CTRL_REG1, &config, 1) == PICO_ERROR_GENERIC) {
        return ACCEL_ERROR_STATUS;
    }

    sleep_ms(10);
    
    return 0;
}

/**
 * @brief Fetch raw data from the device
 * @return Number of bytes read
 */
int Accelerometer::fetch_data()
{
    if (fast_mode) {
        return i2c->read(address, ACCEL_OUT_X_MSB, buffer_short, ACCEL_XYZ_BUFFER);
    }
    else {
        return i2c->read(address, ACCEL_OUT_X_MSB, buffer_full, ACCEL_XYZ_BUFFER_FULL);
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