#ifndef SHARED_RESOURCES_HPP
#define SHARED_RESOURCES_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "mqtt_client.h"

#include "mutex.hpp"
#include "queue.hpp"
#include "semaphore.hpp"

#include "flash.hpp"
#include "shared_buffer.hpp"


#define SHARED_MEMORY_BUFFER_SIZE 50


extern QueueWrapper* accelerometer_side_queue;
extern QueueWrapper* mqtt_transmit_queue;
extern QueueWrapper* mqtt_receive_queue;

extern EventGroupHandle_t network_event_group;

extern esp_mqtt_client_handle_t client;

extern Flash* flash;
extern SharedMemoryBuffer* mqtt_buffer;


#endif
