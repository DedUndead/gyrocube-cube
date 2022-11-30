#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "driver/gpio.h"

#include "vibration_motor.hpp"


static gpio_num_t motor_pin = gpio_num_t(-1);
static TimerHandle_t motor_timer;


static void turn_motor_off(TimerHandle_t xTimer)
{
    if (motor_pin != -1) gpio_set_level(motor_pin, 0);
}

void motor_initialize(const gpio_num_t& pin)
{
    motor_pin = pin;

    gpio_reset_pin(motor_pin);
    gpio_set_direction(motor_pin, GPIO_MODE_OUTPUT);

    motor_timer = xTimerCreate(
		MOTOR_TIMER_TAG,
		pdMS_TO_TICKS(50),  // Arbitrary
		pdFALSE,
		(void*) MOTOR_TIMER_ID,
		turn_motor_off
	);
}

void motor_vibrate(const uint& duration)
{
    xTimerChangePeriod(motor_timer, pdMS_TO_TICKS(duration), portMAX_DELAY);
    xTimerReset(motor_timer, portMAX_DELAY);
    gpio_set_level(motor_pin, 1);
}
