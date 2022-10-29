#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "i2c.hpp"
#include "accelerometer.hpp"
#include "neopixel.hpp"

extern "C" void app_main(void)
{
    NeoPixel led(1, GPIO_NUM_15);

    led.set_pixel(0, 0xff, 0x00, 0x00, 0x00);

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}  
