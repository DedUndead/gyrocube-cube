#include "freertos/event_groups.h"

#include "mutex.hpp"
#include "queue.hpp"
#include "semaphore.hpp"

extern QueueWrapper* accelerometer_side_queue;
extern QueueWrapper* mqtt_transmit_queue;
extern QueueWrapper* mqtt_receive_queue;

extern EventGroupHandle_t network_event_group;

extern esp_mqtt_client_handle_t client = NULL;
