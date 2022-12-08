#include "display_task.hpp"

#include "shared_resources.hpp"
#include "flash.hpp"
#include "led_controller.hpp"
#include "memory_layout_helpers.hpp"

#include <stdio.h>


/*
 * @brief Task that displays current cube's configuration
 */
void v_display_task(void* pvParameters)
{   
    uint8_t new_side;

    LedController leds(LED_STRIP_NUMBER_OF_LEDS, LED_STRIP_PIN);

    init_flash_with_default_configuration(flash);

    while (true) {
        accelerometer_side_queue->consume(&new_side, ACCEL_QUEUE_TIMEOUT);

        cube_config config = load_configuration(flash, new_side);
        leds.fill_color(config.color);
    }
}