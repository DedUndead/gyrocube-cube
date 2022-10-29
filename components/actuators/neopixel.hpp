/**
 * @file NeoPixel.hpp
 * @brief This module provides generic C++ API for NeoPixel LED strips by AdaFruit
 * for ESP-IDF
 * @author Pavel Arefyev
 * Contact: deddh1b@gmail.com
 */
#ifndef NEOPIXEL_HPP
#define NEOPIXEL_HPP


#include "driver/rmt.h"

/* ===== CONSTANTS PARAMETERS ===== */
#define RMT_NUMBER_OF_MEMORY_BLOCK 3    // 3 since we interface RGB strip
#define PIXEL_LENGTH_RMT_SYMBOL    32   // Length of one pixel in RMT symbols
#define RMT_NONE                   0    // Sentinel value for NONE
#define WHITE_PIXEL_OFF            0x00

/* ===== CONFIGURABLE PARAMETERS ===== */
#define RMT_CLOCK_DIVIDER          1   // 80 Mhz / RMT_CLOCK_DIVIDER = Clock Freq MHz
#define MAX_AMOUNT_OF_PIXELS       30  // Each pixel takes 128 bytes of device memory
#define DEFAULT_RMT_CHANNEL        RMT_CHANNEL_0

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
 * Change accorging to your neopixel timings
 * Timings used here are provided in section 10 of example LED:
 * https://cdn-shop.adafruit.com/product-files/2757/p2757_SK6812RGBW_REV01.pdf
 */
enum timing {
    T0H = 24,   // 0 code, high level time
    T0L = 72,   // 0 code, low level time
    T1H = 48,   // 1 code, high level time
    T1L = 48,   // 1 code, low level time
    RST = 6400  // Reset code，low level time
};

/**
 * @brief This class provides abstraction for Addressable LED strip by AdaFruit.
 * The interface set us the correct timings for signaling and provides API
 * for basic color control.
*/
class NeoPixel {
public:
    NeoPixel(const uint8_t& number_of_pixels_,
             const gpio_num_t& pin_,
             const rmt_channel_t& channel_ = DEFAULT_RMT_CHANNEL);
    void refresh();
    void initialize_strip();
    void set_pixel(const uint8_t& index, const uint32_t& rgb, const uint8_t& white = WHITE_PIXEL_OFF);
    void set_pixel(const uint8_t& index,
                   const uint8_t& red,
                   const uint8_t& green,
                   const uint8_t& blue,
                   const uint8_t& white);
private:
    rmt_item32_t rmt_symbols[MAX_AMOUNT_OF_PIXELS * 32];
    const uint8_t number_of_pixels;
    const rmt_channel_t channel;
};


#endif