#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H


#include "pico/stdio.h"
#include "interfaces/i2c.h"

#define ACCEL_DEF_ADDRESS      0x1E
#define ACCEL_CTRL_REG1        0x2A
#define ACCEL_OUT_X_MSB        0x01
#define ACCEL_SCALE_FACTOR     (float)0.244

#define ACCEL_XYZ_BUFFER       3
#define ACCEL_XYZ_BUFFER_FULL  6
#define ACCEL_ERROR_STATUS     0xEFFF
#define ACCEL_ORIENT_UNKNOWN   0x06
#define ACCEL_THRESHOLD        850

typedef struct acc_measurements {
    int16_t x;
    int16_t y;
    int16_t z;
} acc_measurements;

/**
 * @brief Abstraction of accelerometer class
 * Provides interface to read/config values on chip
 */
class Accelerometer {
public:
    Accelerometer(I2C* i2c_, uint8_t address_ = DEF_ADDRESS);
    int read(acc_measurements& buffer);
    int enable_in_fast_mode();
    int enable();
    int disable();
    uint8_t get_side();
private:
    int fetch_data();

    bool fast_mode;                             // Flag determines fast mode operation
    uint8_t buffer_short[ACCEL_XYZ_BUFFER];     // Short buffer for fast reading
    uint8_t buffer_full[ACCEL_XYZ_BUFFER_FULL]; // Full buffer for full reading

    acc_measurements internal_measurements;
    uint8_t address;
    I2C* i2c;
};


#endif