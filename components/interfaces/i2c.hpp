#ifndef I2C_HPP
#define I2C_HPP


#include <stdio.h>
#include "driver/i2c.h"
#include "esp_err.h"


#define I2C_DEFAULT_HW_BLOCK I2C_NUM_0
#define I2C_DEFAULT_SDA_PIN GPIO_NUM_21
#define I2C_DEFAUL_SCL_PIN GPIO_NUM_22
#define I2C_MAX_TX_BUFFER_LEN 255


class I2C {
public:
    I2C(const uint32_t& frequency,
        const uint8_t& sda_pin = I2C_DEFAULT_SDA_PIN,
        const uint8_t& scl_pin = I2C_DEFAUL_SCL_PIN,
        const uint8_t& hw_block_number = I2C_DEFAULT_HW_BLOCK);
    esp_err_t write_register(const uint8_t& slave_addr, 
                             const uint8_t& reg_addr,
                             const uint8_t* data,
                             const uint8_t& tx_len,
                             const uint& timeout) const;
    esp_err_t read_register(const uint8_t& slave_addr,
                            const uint8_t& reg_addr,
                            uint8_t* buffer,
                            const uint8_t& rx_len,
                            const uint& timeout_ms) const;
private:
    const uint8_t hw_block;
};


#endif