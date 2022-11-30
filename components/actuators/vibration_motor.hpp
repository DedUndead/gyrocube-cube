#ifndef VIBRATION_MOTOR_CPP
#define VIBRATION_MOTOR_CPP

#include "driver/gpio.h"

#define MOTOR_TIMER_TAG  "MOTORTIMER"
#define MOTOR_TIMER_ID   1

void motor_initialize(const gpio_num_t& pin);
void motor_vibrate(const uint& duration);


#endif