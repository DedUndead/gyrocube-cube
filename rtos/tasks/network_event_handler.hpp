#ifndef NETWORK_EVENT_HANDLER_HPP


/* This module contains event handlers for Wi-Fi and Mqtt event loop */

esp_err_t network_event_handler(void *ctx, system_event_t *event);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_app_start(void);
void init_wifi_mqtt(void);

extern bool mqtt_connected = false;


#define NETWORK_EVENT_HANDLER_HPP