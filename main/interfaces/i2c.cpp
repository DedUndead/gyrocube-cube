#include "driver/i2c.h"

#define I2C_DEFAULT_HW_BLOCK = 0
#define I2C_DEFAULT_SDA_PIN = 21
#define I2C_DEFAUL_SCL_PIN = 22
#define I2C_MAX_TX_BUFFER_LEN = 255


class I2C {
public:
    I2C(const int& frequenvy,
        const uint8_t& sda_pin,
        const uint8_t& scl_pin,
        const uint8_t& hw_block_number);
    esp_err_t write_register(const uint8_t& slave_addr, 
                             const uint16_t& reg_addr,
                             const uint8_t* data,
                             const uint8_t& tx_len,
                             const uint& timeout) const;
    esp_err_t read_register(const uint8_t& slave_addr,
                            const uint16_t& reg_addr,
                            uint8_t* buffer,
                            const uint8_t& rx_len,
                            const uint& timeout_ms) const;
private:
    const uint8_t hw_block;
};

I2C::I2C(const int& frequency,
         const uint8_t& sda_pin = I2C_DEFAULT_SDA_PIN,
         const uint8_t& scl_pin = I2C_DEFAUL_SCL_PIN,
         const uint8_t& hw_block_number = I2C_DEFAULT_HW_BLOCK) 
{
    hw_block = hw_block_number
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = frequency,
    };

    i2c_param_config(hw_block, &conf);
    // Disable buffering as I2C is configured in master mode
    i2c_driver_install(hw_block, conf.mode, 0, 0, 0);
}

esp_err_t I2C::write_register(const uint8_t& slave_addr,
                              const uint16_t& reg_addr,
                              const uint8_t* data,
                              const uint8_t& tx_len,
                              const uint& timeout) const
{
    uint8_t write_buf[I2C_MAX_TX_BUFFER_LEN];
    
    // Include register address into write buffer
    write_buf[0] = reg_addr;
    for (uint8_t i = 1; i < tx_len; i++) {
        write_buf[i] = data[i - 1];
    }

    return i2c_master_write_to_device(
        hw_block, slave_addr, write_buf, sizeof(write_buf), timeout / portTICK_PERIOD_MS
    );;
}

esp_err_t I2C::read_register(const uint8_t& slave_addr,
                             const uint16_t& reg_addr,
                             uint8_t* buffer,
                             const uint8_t& rx_len,
                             const uint& timeout_ms) const
{
    return i2c_master_write_read_device(
        hw_block, slave_addr, &reg_addr, 1, data, rx_len, timeout_ms / portTICK_PERIOD_MS
    );
}