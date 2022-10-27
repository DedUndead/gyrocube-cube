#include "i2c.hpp"
#include "driver/i2c.h"
#include "esp_log.h"

I2C::I2C(const uint32_t& frequency,
         const uint8_t& sda_pin,
         const uint8_t& scl_pin,
         const uint8_t& hw_block_number) : hw_block(hw_block_number)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master = { .clk_speed = frequency },  
        .clk_flags = 0
    };

    ESP_ERROR_CHECK(i2c_param_config(hw_block, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(hw_block, conf.mode, 0, 0, 0));
}

esp_err_t I2C::write_register(const uint8_t& slave_addr,
                              const uint8_t& reg_addr,
                              const uint8_t* data,
                              const uint8_t& tx_len,
                              const uint& timeout) const
{
    uint8_t write_buf[I2C_MAX_TX_BUFFER_LEN];
    
    // Include register address into write buffer
    write_buf[0] = reg_addr;
    for (uint8_t i = 1; i < tx_len + 1; i++) {
        write_buf[i] = data[i - 1];
    }

    esp_err_t res = i2c_master_write_to_device(
        hw_block, slave_addr, write_buf, tx_len + 1, timeout / portTICK_PERIOD_MS
    );

    return res;
}

esp_err_t I2C::read_register(const uint8_t& slave_addr,
                             const uint8_t& reg_addr,
                             uint8_t* buffer,
                             const uint8_t& rx_len,
                             const uint& timeout_ms) const
{
    esp_err_t res = i2c_master_write_read_device(
        hw_block, slave_addr, &reg_addr, 1, buffer, rx_len, timeout_ms / portTICK_PERIOD_MS
    );

    return res;
}