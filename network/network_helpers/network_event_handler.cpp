/* 
 * This module contains event handlers for Wi-Fi and Mqtt event loop
 * There is not Cpp wrapper written for MQTT. Could be done, but there is not particular 
 * reason, since the functionality is event-based.
 */
#include "network_event_handler.hpp"
#include "network_definitions.hpp"
#include "shared_resources.hpp"
#include "memory_layout_helpers.hpp"
#include "network_configuration/netconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_err.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include <stdio.h>
#include <string.h>


/* On connect, send cube configuration to mqtt queue */
static void send_cube_config()
{
    for (int i = 1; i <= 6; i++) {
        cube_config conf = load_configuration(flash, i);

        char data[MAX_MESSAGE_BUFFER_SIZE];
        char topic[] = MQTT_TOPIC_CONFIG;

        sprintf(data, "{tag:%02Xcolor:%06X}", message_tag::CUBE_SIDE_CONFIG, conf.color);
        int data_size = strlen(data);
        char* mqtt_data = mqtt_buffer->insert(data, data_size);

        message msg = {
            .data = mqtt_data,
            .topic = topic,
            .data_len = data_size
        };
        mqtt_transmit_queue->send_back(&msg);
    }
}

/* On connect, send cube configuration to mqtt queue */
static void send_network_joined()
{
    for (int i = 1; i <= 6; i++) {
        char data[MAX_MESSAGE_BUFFER_SIZE];
        char topic[] = MQTT_TOPIC_CONFIG;

        sprintf(data, "{tag:%02Xaddress:%04X}", message_tag::CUBE_JOINED, 0xFFFF);
        int data_size = strlen(data);
        char* mqtt_data = mqtt_buffer->insert(data, data_size);

        message msg = {
            .data = mqtt_data,
            .topic = topic,
            .data_len = data_size
        };
        mqtt_transmit_queue->send_back(&msg);
    }
}


/*
 * @brief Event handler registered to receive MQTT events
 * This function is called by the MQTT client event loop.
 */
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(NETWORK_LOG_TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);

    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    client = event->client;

    int msg_id;
    message msg;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(NETWORK_LOG_TAG, "MQTT_EVENT_CONNECTED");

        xEventGroupSetBits(network_event_group, MQTT_CONNECTED_NETWORK_EVENT_BIT);
        // Subscribe to required topics
        msg_id = esp_mqtt_client_subscribe(client, "gyro/cube", 0);
        ESP_LOGI(NETWORK_LOG_TAG, "sent subscribe successful, msg_id=%d", msg_id);

        //send_cube_config();
        //send_network_joined();

        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(NETWORK_LOG_TAG, "MQTT_EVENT_DISCONNECTED");

        xEventGroupClearBits(network_event_group, MQTT_CONNECTED_NETWORK_EVENT_BIT);

        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(NETWORK_LOG_TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);

        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(NETWORK_LOG_TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);

        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(NETWORK_LOG_TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);

        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(NETWORK_LOG_TAG, "MQTT_EVENT_DATA");

        msg = {
            .data = event->data,
            .topic = event->topic,
            .data_len = event->data_len
        };
        mqtt_receive_queue->send_back(&msg);

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(NETWORK_LOG_TAG, "MQTT_EVENT_ERROR");

        break;
    default:
        ESP_LOGI(NETWORK_LOG_TAG, "Other event id:%d", event->event_id);

        break;
    }
}

/* Start MQTT application */
static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqttConfig;
    mqttConfig.uri = MQTT_URI;
    mqttConfig.host = MQTT_HOST;
    mqttConfig.event_handle = NULL;
    
    client = esp_mqtt_client_init(&mqttConfig);
    esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

/*
 * @brief Simple event handler to monitor the wifi connection
 */
void network_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    // STA configured state
    case WIFI_EVENT_STA_START:
        esp_wifi_connect();
        ESP_LOGI(NETWORK_LOG_TAG, "Trying to connect with Wi-Fi\n");
        break;
    // STA connected to AP state
    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(NETWORK_LOG_TAG, "Wi-Fi connected\n");
        break;
    // STA received IP address state 
    case IP_EVENT_STA_GOT_IP:
        ESP_LOGI(NETWORK_LOG_TAG, "Got IP: starting MQTT Client\n");
        mqtt_app_start();
        break;
    // Connection lost state
    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI(NETWORK_LOG_TAG, "Disconnected: Retrying Wi-Fi\n");
        esp_wifi_connect();
        break;
    default:
        break;
    }
}

/* Initialize endpoint with Wi-Fi and Mqtt stack */
void init_wifi_mqtt(void)
{
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t wifi_event_handler_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &network_event_handler,
                                                        NULL,
                                                        &wifi_event_handler_instance));
    esp_event_handler_instance_t ip_event_handler_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &network_event_handler,
                                                        NULL,
                                                        &ip_event_handler_instance));

    wifi_config_t wifi_config = {
        .sta = {
            { .ssid = SSID },
            { .password = PSWD },
            .threshold = {
                .authmode = WIFI_AUTH_WPA2_PSK,
            }
        }
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}
