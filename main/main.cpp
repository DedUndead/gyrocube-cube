#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "i2c.hpp"
#include "accelerometer.hpp"
#include "led_controller.hpp"

extern "C" void app_main(void)
{
    LedController leds(20, GPIO_NUM_4);

    while (1) {    
        leds.gradient(0xff0000, 0x0000ff, 1500);
        leds.gradient(0x0000ff, 0xff0000, 1500);
    }
}  
