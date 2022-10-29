#include "led_controller.hpp"
#include "neopixel.hpp"

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
