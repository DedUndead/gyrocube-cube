#include "led_controller.hpp"
#include "neopixel.hpp"
#include "freertos/task.h"

LedController::LedController(const uint8_t& number_of_pixels_, const gpio_num_t& pin_, const rmt_channel_t& channel_) :
    number_of_pixels(number_of_pixels_), strip(number_of_pixels_, pin_, channel_)
{ /* Empty constructor body */ }

/**
 * @brief Fill LED strip with specified color in RGB scheme 
 */
void LedController::fill_color(const uint32_t& rgb, const uint8_t& white)
{
    fill_color(rgb >> 16, rgb >> 8, rgb, white);
}
void LedController::fill_color(const uint8_t& red, const uint8_t& green, const uint8_t& blue, const uint8_t& white)
{
    for (int i = 0; i < number_of_pixels; i++) {
        strip.set_pixel(i, red, green, blue, white);
    }
    strip.refresh();
}

/**
 * @brief Move from one color to another smoothly
 */
void LedController::gradient(const uint32_t& start_color,
                             const uint32_t& end_color,
                             const uint16_t duration_ms,
                             const uint8_t& white)
{
    const uint8_t fraction_step = 1;
    const uint8_t num_steps = 100 / fraction_step;
    const uint8_t delay_ms = duration_ms / num_steps;

    for (uint8_t fraction = 0; fraction < 100 + 1; fraction += fraction_step) {
        fill_color(LedController::interpolate(start_color, end_color, fraction), white);
        vTaskDelay(delay_ms / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Dimming effect 
 */
void LedController::dimming(const uint32_t& rgb, const uint16_t duration_ms, const uint8_t& white)
{   
    gradient(rgb, 0, duration_ms, white);
}

/**
 * @brief Appear effect 
 */
void LedController::appearing(const uint32_t& rgb, const uint16_t duration_ms, const uint8_t& white)
{   
    gradient(0, rgb, duration_ms, white);
}

/**
 * @brief Interpolate two RGB colors
 * @param color1   Starting color
 * @param color2   Ending color
 * @param fraction Interpolation percent
 */
uint32_t LedController::interpolate(const uint32_t& color1, const uint32_t& color2, const uint8_t& fraction)
{
    uint8_t r1 = (color1 >> 16);
    uint8_t r2 = (color2 >> 16);
    uint8_t g1 = (color1 >> 8);
    uint8_t g2 = (color2 >> 8);
    uint8_t b1 = color1;
    uint8_t b2 = color2;

    return ((r2 - r1) * fraction / 100 + r1) << 16 |
           ((g2 - g1) * fraction / 100 + g1) << 8  |
           ((b2 - b1) * fraction / 100 + b1);
}
