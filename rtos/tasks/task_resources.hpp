#include "freertos/event_groups.h"

#include "mutex.hpp"
#include "queue.hpp"
#include "semaphore.hpp"

extern QueueWrapper* accelerometer_side_queue;
extern QueueWrapper* mqtt_message_queue;

extern EventGroupHandle_t network_event_group;
