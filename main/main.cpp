#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "shared_resources.hpp"
#include "accelerometer_task.hpp"
#include "display_task.hpp"
#include "network_event_handler.hpp"
#include "network_definitions.hpp"

#include "i2c.hpp"
#include "accelerometer.hpp"
#include "led_controller.hpp"
#include "vibration_motor.hpp"
#include "flash.hpp"
#include "shared_buffer.hpp"

#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "mqtt_client.h"

#define STANDALONE

// Extern resources declaration
QueueWrapper* accelerometer_side_queue;
QueueWrapper* mqtt_receive_queue;
QueueWrapper* mqtt_transmit_queue;
esp_mqtt_client_handle_t client;
EventGroupHandle_t network_event_group;
Flash* flash;
SharedMemoryBuffer* mqtt_buffer;


/* WARNING: Call before scheduler is started */
static void init_rtos_shared_resources()
{
    nvs_flash_init();

    const uint16_t default_service_queue_size = 10;

    // Ignore -Wunused-variable; since shared resources are declared extern
    accelerometer_side_queue = new QueueWrapper(default_service_queue_size, sizeof(uint8_t));
    mqtt_transmit_queue = new QueueWrapper(default_service_queue_size, sizeof(message));
    mqtt_receive_queue = new QueueWrapper(default_service_queue_size, sizeof(message));

    flash = new Flash;
    mqtt_buffer = new SharedMemoryBuffer(SHARED_MEMORY_BUFFER_SIZE);

    client = NULL;
}


extern "C" void app_main(void)
{
    init_rtos_shared_resources();
    #ifndef STANDALONE
    init_wifi_mqtt();  // FIXME: Board issue?
    #endif

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
