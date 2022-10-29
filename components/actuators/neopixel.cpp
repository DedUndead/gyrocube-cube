/**
 * @file NeoPixel.cpp
 * @brief This module provides generic C++ API for NeoPixel LED strips by AdaFruit
 * for ESP-IDF. Partially adopted from https://github.com/mirronelli/neopixel/blob/master/main/neopixel.cpp
 * @author Pavel Arefyev
 * Contact: deddh1b@gmail.com
 */
#include "neopixel.hpp"

#include <unistd.h>
#include "driver/rmt.h"
#include "esp_err.h"
#include "esp_log.h"

/**
 * Gamma correction table
 * Gamma correction is necessary to transmit true tone of the colors
 * Read more about gamma correction effect:
 * https://cdn-learn.adafruit.com/downloads/pdf/led-tricks-gamma-correction.pdf 
 * NOTE: This takes 255 bytes of memory, remove or use dynamic floating point calculation
 */
static const uint8_t gamma8[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
    115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
    144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
    177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255 
};

NeoPixel::NeoPixel(const uint8_t& number_of_pixels_, const gpio_num_t& pin_, const rmt_channel_t& channel_) :
    number_of_pixels(number_of_pixels_), channel(channel_)
{
    // Configure Remote Control Tranceiver
    rmt_config_t rmt_conf = RMT_DEFAULT_CONFIG_TX(pin_, channel);
    rmt_conf.clk_div = 1;

    ESP_ERROR_CHECK(rmt_config(&rmt_conf));
    ESP_ERROR_CHECK(rmt_driver_install(rmt_conf.channel, 0, 0));

    for (int i = 0; i < number_of_pixels * PIXEL_LENGTH_RMT_SYMBOL; i++) {
        rmt_symbols[i].level0 = 1;  // First signal of symbols to be HIGH
        rmt_symbols[i].level1 = 0;  // Second signal of symbols to be LOW
    }

    clear_strip();
}

// Each pixel is encoded with 32 rmt symbols, and each symbol contains two configurable composites
// Thus, 8 rmt symbols correspond to one color byte. Color scheme is GRBW
//
// |                              PIXEL                               | 1024bits on device memory
// | Symbol | Symbol | Symbol | Symbol | Symbol |     ..     | Symbol | 1024bits on device memory
//
// |                              Symbol                              | 32bits on device memory 
// | duration0 (defines logic -> timing enum) | level0 (signal level) | 16bits on device memory
// | duration1 (defines logic -> timing enum) | level1 (signal level) | 16bits on device memory
//
// In other words, each DATA BIT is encoded with two physical signals of various duration.
//
// Refer for RMT symbols structure description: 
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/rmt.html

/*
 * @brief Clear LED strip RMT components
 */
void NeoPixel::clear_strip()
{
    for (int i = 0; i < number_of_pixels * PIXEL_LENGTH_RMT_SYMBOL; i++) {
        rmt_symbols[i].duration0 = timing::T0H;  // Set signal to represent logical 0 
        rmt_symbols[i].duration1 = timing::T0L;  // Set signal to represent logical 0
    }

    refresh();
}

/**
 * @brief Refresh LED strip with new data
*/
void NeoPixel::refresh()
{
    usleep(timing::RST * RMT_CLOCK_DIVIDER / RMT_CLOCK_FREQUENCY_MHZ);
    ESP_ERROR_CHECK(rmt_write_items(channel, rmt_symbols, number_of_pixels * PIXEL_LENGTH_RMT_SYMBOL, true));
}

/**
 * @brief Set pixel color with RGB colorcode
 * @param index Index of the pixel in the strip
 * @param rgb   RGB colorcode
 * @param white White LED intensity 
 */
void NeoPixel::set_pixel(const uint8_t& index, const uint32_t& rgb, const uint8_t& white)
{
    set_pixel(index, rgb >> 16, rgb >> 8, rgb, white);
}

/**
 * @brief Set pixel color
 * @param index Index of the pixel in the strip
 */
void NeoPixel::set_pixel(const uint8_t& index,
                         const uint8_t& red,
                         const uint8_t& green,
                         const uint8_t& blue,
                         const uint8_t& white)
{
    if (index >= number_of_pixels) return;

    const uint32_t pixel_color = (gamma8[green] << 24) | 
                                 (gamma8[red]   << 16) | 
                                 (gamma8[blue]  << 8)  | 
                                 (gamma8[white]);

    uint32_t data_bit_selector_mask = 1 << (PIXEL_LENGTH_RMT_SYMBOL - 1);
    const uint16_t start_bit = index * PIXEL_LENGTH_RMT_SYMBOL;

    for (int i = start_bit; i < start_bit + PIXEL_LENGTH_RMT_SYMBOL; i++) {
        // Encode 1
        if (pixel_color & data_bit_selector_mask) {
            rmt_symbols[i].duration0 = timing::T1H;
            rmt_symbols[i].duration1 = timing::T1L;
        }
        // Encode 0
        else {
            rmt_symbols[i].duration0 = timing::T0H;
            rmt_symbols[i].duration1 = timing::T0L;
        }

        data_bit_selector_mask >>= 1;  // Move to next bit
    }
}
