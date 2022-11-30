/* 
 * This module contains event handlers for Wi-Fi and Mqtt event loop
 * There is not Cpp wrapper written for MQTT. Could be done, but there is not particular 
 * reason, since the functionality is event-based.
 */
#include "network_event_handler.hpp"
#include "network_definitions.hpp"
#include "shared_resources.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"


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

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(NETWORK_LOG_TAG, "MQTT_EVENT_CONNECTED");

        xEventGroupSetBits(network_event_group, MQTT_CONNECTED_NETWORK_EVENT_BIT);
        // Subscribe to required topics
        msg_id = esp_mqtt_client_subscribe(client, "/topic/test1", 0);
        ESP_LOGI(NETWORK_LOG_TAG, "sent subscribe successful, msg_id=%d", msg_id);

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

        message msg = {
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
    ESP_LOGI(NETWORK_LOG_TAG, "STARTING MQTT");
    esp_mqtt_client_config_t mqttConfig;
    mqttConfig.uri = "mqtt://192.168.1.4:1883";
    
    client = esp_mqtt_client_init(&mqttConfig);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

/*
 * @brief Simple event handler to monitor the mqtt/wifi connection
 */
esp_err_t network_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    // STA configured state
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        ESP_LOGI(NETWORK_LOG_TAG, "Trying to connect with Wi-Fi\n");
        break;
    // STA connected to AP state
    case SYSTEM_EVENT_STA_CONNECTED:
        ESP_LOGI(NETWORK_LOG_TAG, "Wi-Fi connected\n");
        break;
    // STA received IP address state 
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(NETWORK_LOG_TAG, "Got IP: starting MQTT Client\n");
        mqtt_app_start();
        break;
    // Connection lost state
    case SYSTEM_EVENT_STA_DISCONNECTED:
        ESP_LOGI(NETWORK_LOG_TAG, "Disconnected: Retrying Wi-Fi\n");
        esp_wifi_connect();
        break;

    default:
        break;
    }
    return ESP_OK;
}

/* Initialize endpoint with Wi-Fi and Mqtt stack */
void init_wifi_mqtt(void)
{
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_loop_init(network_event_handler, (void*)NULL));

    wifi_config_t wifi_config;
    wifi_config.sta = {
        .ssid = "V46D-1",
        .password = "2483124831",
        .threshold = {
            .authmode = WIFI_AUTH_WPA2_PSK,
        }
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start());
}
