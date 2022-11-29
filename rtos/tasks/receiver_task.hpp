#ifndef RECEIVER_TASK_HPP
#define REVEIVER_TASK_HPP


#define MAX_NUMBER_OF_TOKENS 10
#define MIN_NUMBER_OF_TOKENS 4

#define MOTOR_PIN 4
#define MOTOR_DEFAULT_DURATION_MS 200

// Use Jasmine as multi-include
#define JSMN_HEADER
#include "jsmn.h"


void v_receiver_task(void* pvParameters);


#endif