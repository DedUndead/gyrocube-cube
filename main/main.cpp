#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "i2c.hpp"
#include "accelerometer.hpp"
#include "led_controller.hpp"

extern "C" void app_main(void)
{
    LedController leds(20, GPIO_NUM_4);

    leds.fill_color(0xff, 0x00, 0x00);

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}  
