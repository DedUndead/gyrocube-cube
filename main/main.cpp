#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "shared_resources.hpp"
#include "accelerometer_task.hpp"
#include "display_task.hpp"
#include "network_event_handler.hpp"

#include "esp_log.h"

#include "i2c.hpp"
#include "accelerometer.hpp"
#include "led_controller.hpp"
#include "vibration_motor.hpp"
#include "flash.hpp"
#include "driver/gpio.h"
#include "mqtt_client.h"

// Extern resources declaration
QueueWrapper* accelerometer_side_queue;
esp_mqtt_client_handle_t client;

static void init_rtos_shared_resources()
{
    const uint16_t default_serive_queue_size = 10;

    // Ignore -Wunused-variable since shared resources are declared extern
    accelerometer_side_queue = new QueueWrapper(default_serive_queue_size, sizeof(uint8_t));
    client = NULL;
}


extern "C" void app_main(void)
{
    init_rtos_shared_resources();
    init_wifi_mqtt();

    // Task sizes were identified imperatively and as multiples of configMINIMAL_STACK_SIZE
    
    xTaskCreate(v_accelerometer_task, 
                "Accelerometer Task",
                configMINIMAL_STACK_SIZE * 3,
                NULL,
                (tskIDLE_PRIORITY + 1UL),
                (TaskHandle_t *)NULL);
    
    xTaskCreate(v_display_task,
                "Display Task",
                configMINIMAL_STACK_SIZE * 12,
                NULL,
                (tskIDLE_PRIORITY + 1UL),
                (TaskHandle_t *)NULL);

    // Note: Schedulered is called automatically - ESP-IDF feature
    // Refer to: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html
}  
