#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "interfaces/i2c.hpp"


extern "C" void app_main(void)
{
    I2C i2c(100000);

    while (1) {
        ESP_LOGI("INFO", "Running main.");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
