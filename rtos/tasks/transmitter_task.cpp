#include "shared_resources.hpp"
#include "network_definitions.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define MAX_NUMBER_OF_TOKENS 10

// Use Jasmine as multi-include
#define JSMN_HEADER
#include "jsmn.h"

/*
 * @brief Receiver task consumes raw arriving data
 * It decapsulates the frames and dispatches events based on the message type
 */
void v_receiver_task(void* pvParameters)
{   
    while (true) {
        message msg = mqtt_transmit_queue->consume();
        xEventGroupWaitBits(network_event_group, MQTT_CONNECTED_NETWORK_EVENT_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
        
        esp_mqtt_client_publish(client, "/cube", "Helllo World", 0, 0, 0);
    }
}