#ifndef I2C_HPP
#define I2C_HPP


#define I2C_DEFAULT_HW_BLOCK = 0
#define I2C_DEFAULT_SDA_PIN = 21
#define I2C_DEFAUL_SCL_PIN = 22
#define I2C_MAX_TX_BUFFER_LEN = 255


class I2C {
public:
    I2C(const int& frequency,
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


#endif