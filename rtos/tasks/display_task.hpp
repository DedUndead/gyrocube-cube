#ifndef DISPLAY_TASK_HPP
#define DISPLAY_TASK_HPP

#include <stdint.h>
#include "driver/gpio.h"

#define ACCEL_QUEUE_TIMEOUT 100

#define LED_STRIP_PIN GPIO_NUM_5
#define LED_STRIP_NUMBER_OF_LEDS 30

void v_display_task(void* pvParameters);


#endif