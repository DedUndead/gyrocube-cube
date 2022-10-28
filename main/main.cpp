#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "i2c.hpp"
#include "accelerometer.hpp"


extern "C" void app_main(void)
{
    I2C i2c(250 * 1000);
    Accelerometer accelerometer(&i2c);

    accelerometer.enable();

    while (1) {
        ESP_LOGI("ACCEL", "%d", accelerometer.get_side());
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}  
