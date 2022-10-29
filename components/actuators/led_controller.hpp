#ifndef LED_CONTROLLER
#define LEF_CONTROLLER


#include "neopixel.hpp"

/**
 * @brief This class provides effects functionality for NeoPixel LED.
 * High level abstraction allows to control effects without going into
 * RMT implementation details. 
 */
class LedController {
public:
    LedController(const uint8_t& number_of_pixels_,
                  const gpio_num_t& pin_,
                  const rmt_channel_t& channel_ = DEFAULT_RMT_CHANNEL);
    void fill_color(const uint8_t& red, const uint8_t& green, const uint8_t& blue, const uint8_t& white = WHITE_PIXEL_OFF);
    void fill_color(const uint32_t& rgb, const uint8_t& white = WHITE_PIXEL_OFF);

    // Effects
    void gradient(const uint32_t& start_color,
                  const uint32_t& end_color,
                  const uint16_t duration_ms,
                  const uint8_t& white = WHITE_PIXEL_OFF);
    void dimming(const uint32_t& rgb, const uint16_t duration_ms, const uint8_t& white = WHITE_PIXEL_OFF);
    void appearing(const uint32_t& rgb, const uint16_t duration_ms, const uint8_t& white = WHITE_PIXEL_OFF);
    void pulsing(const uint32_t& rgb, 
                 const uint16_t duration_ms,
                 const uint16_t pulse_duration_ms,
                 const uint8_t& white = WHITE_PIXEL_OFF);

    static uint32_t interpolate(const uint32_t& color1, const uint32_t& color2, const uint8_t& fraction); 
private:
    const uint8_t number_of_pixels;
    NeoPixel strip;
};


#endif