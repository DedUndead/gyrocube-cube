#include "display_task.hpp"
#include "shared_resources.hpp"
#include "flash.hpp"
#include "led_controller.hpp"

#include <stdio.h>

static void init_flash_with_default_configuration(Flash& flash)
{
    const uint8_t number_of_sides = 6;
    const uint32_t default_colors[number_of_sides] = { 0x0000ff, 0x00ff00, 0xff0000,
                                                       0xff00ff, 0x00ffff, 0xffff00 };

    for (int i = 1; i < number_of_sides + 1; i++) {
        cube_config config = { .color = default_colors[i - 1] };
        char key = i + '0';

        flash.write(&key, &config, sizeof(cube_config));
    }
}

static cube_config load_configuration(Flash& flash, const uint8_t& side)
{
    cube_config config;
    size_t length_read;
    char key = side + '0';

    flash.read(&key, &config, &length_read);

    return config;
}

/*
 * @brief Task that displays current cube's configuration
 */
void v_display_task(void* pvParameters)
{   
    uint8_t new_side;

    Flash flash;
    LedController leds(LED_STRIP_NUMBER_OF_LEDS, LED_STRIP_PIN);

    init_flash_with_default_configuration(flash);

    while (true) {
        accelerometer_side_queue->consume(&new_side, ACCEL_QUEUE_TIMEOUT);

        cube_config config = load_configuration(flash, new_side);
        leds.fill_color(config.color);
    }
}