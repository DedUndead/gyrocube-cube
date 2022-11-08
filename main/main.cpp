#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "i2c.hpp"
#include "accelerometer.hpp"
#include "led_controller.hpp"
#include "driver/gpio.h"


void led_test()
{
    LedController leds(20, GPIO_NUM_4);

    while (1) {    
        leds.gradient(0xff0000, 0x0000ff, 1500);
        leds.gradient(0x0000ff, 0xff0000, 1500);
    }
}

void i2c_test()
{
    I2C i2c(250 * 1000);
    Accelerometer accel(&i2c);

    accel.enable();
    while (1) {
        ESP_LOGI("ACCEL", "%d", accel.get_side());
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void motor_test()
{
    gpio_num_t motor = GPIO_NUM_4;

    gpio_reset_pin(motor);
    gpio_set_direction(motor, GPIO_MODE_OUTPUT);

    while(1) {
        gpio_set_level(motor, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(motor, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


extern "C" void app_main(void)
{
    i2c_test();
}  
