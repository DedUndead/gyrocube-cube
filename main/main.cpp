#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"


extern "C" void app_main(void)
{
    while (1) {
        ESP_LOGI("INFO", "Running main.");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
