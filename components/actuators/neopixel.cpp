/**
 * @file NeoPixel.cpp
 * @brief This module provides generic C++ API for NeoPixel LED strips by AdaFruit
 * for ESP-IDF
 * @author Pavel Arefyev
 * Contact: deddh1b@gmail.com
 */
#include "driver/rmt.h"
#include "esp_err.h"

/* ===== CONSTANTS PARAMETERS ===== */
#define RMT_NUMBER_OF_MEMORY_BLOCK 3   // 3 since we interface RGB strip
#define PIXEL_LENGTH_RMT_SYMBOL    32  // Length of one pixel in RMT symbols

/* ===== CONFIGURABLE PARAMETERS ===== */
#define RMT_CLOCK_DIVIDER          10  // 80 Mhz / RMT_CLOCK_DIVIDER = Clock Freq MHz
#define MAX_AMOUNT_OF_PIXELS       30  // Each pixel takes 128 bytes of device memory

/**
 * @brief Timings are defined for each specific Adafruit addressable strips
 * They may differ, the strip used in the project is SK6812, refer to the datasheet
 * 
 * The timings describe duration in RMT tick
 * Default RMT clock source provides frequency of 80 MHz
 * To calculate the amount of ticks N to describe the timing in microseconds T :
 *     
 *     N = T * (80 / clk_div)
 * 
 * Clock divider is set during RMT channel configuration
 * 
 * For better understanding of Remote Control Tranceiver in ESP32, refer to
 * https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/peripherals/rmt.html
 * 
*/
// Change accorging to your neopixel timings
enum timing {
    T0H = 3;   // 0 code, high level time
    T0L = 9;   // 0 code, low level time
    T1H = 6;   // 1 code, high level time
    T1L = 6;   // 1 code, low level time
    RST = 800  // Reset code，low level time
}

/**
 * @brief This class provides abstraction for Addressable LED strip by AdaFruit.
 * The interface set us the correct timings for signaling and provides API
 * for basic color control.
*/
class NeoPixel {
public:
    NeoPixel(const uint8_t& number_of_pixels_, const uint8_t& pin_, const rmt_channel& channel_);
    void initialize_strip();
private:
     const uint8_t number_of_pixels; 
    // Each pixel color is described with 32 rmt symbols
    rmt_item32_t rmt_symbols[MAX_AMOUNT_OF_PIXELS * 32];  // TODO: Change to dynamic allocation if called before scheduler
};

NeoPixel::NeoPixel(const uint8_t& number_of_pixels_, const uint8_t& pin_, const rmt_channel& channel_) :
    number_of_pixels(number_of_pixels_)
{
    // Configure Remote Control Tranceiver
    rmt_config_t rmt_conf = {
        .rmt_mode = RMT_MODE_TX,
        .channel = channel,
        .gpio_pin = pin,
        .mem_block_num = NUMBER_OF_MEMORY_BLOCK,
        .tx_config = {
            .loop_en = false,
            .carrier_en = false,
            .idle_output_en = true,
            .idle_level = RMT_IDLE_LEVEL_LOW,
        }
        .clk_div = RMT_CLOCK_DIVIDER
    };

    ESP_ERROR_CHECK(rmt_config(&rmt_conf));
    ESP_ERROR_CHECK(rmt_driver_install(rmt_conf.channel, 0, 0));

    initialize_strip();
}

/*
 * @brief Initialize LED strip RMT components
 */
void NeoPixel::initialize_strip()
{
    // Each pixel is represented with 32 rmt symbols, and each symbol contains two configurable composites
    //
    // |                              PIXEL                               | 1024 bits
    // | Symbol | Symbol | Symbol | Symbol | Symbol |     ..     | Symbol | 1024 bits 
    //
    // |                              Symbol                              | 32bits
    // | duration0 (defines logic -> timing enum) | level0 (signal level) | 16bits
    // | duration1 (defines logic -> timing enum) | level1 (signal level) | 16bits
    //
    // Refer for RMT symbols structure description: 
    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/rmt.html

    for (int i = 0; i < number_of_pixels * PIXEL_LENGTH_RMT_SYMBOL; i++) {
        rmt_symbol[index] = {
            .level0 = 1,  // First signal of symbol to be HIGH
            .duration0 = timing::T0H  // Set signal to represent logical 0 
            .level1 = 0;  // Second signal of symbol to be LOW
            .duration1 = timing::T0L  // Set signal to represent logical 0
        }
    }
}