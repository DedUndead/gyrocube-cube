#include "i2c.hpp"
#include "accelerometer.hpp"

#include "freertos/timers.h"


#define I2C_BUS_FREQUENCY 250 * 1000

#define SAMPLE_TIMER_TAG "ACCSAMPLE"
#define SAMPLE_TIMER_ID  0
#define SAMPLE_TIMER_MS  100

#define BUFFER_WINDOW_SIZE 3
static BinarySemaphore* ready_to_sample;


static void sample_cube_side(TimerHandle_t xTimer)
{
    if (ready_to_sample != nullptr) { 
	    ready_to_sample.give();
    }
}

static bool all_equal(uint8_t* buffer, const uint8_t& size)
{
    for (int i = 1; i < size; i++) {
        if (buffer[i] != buffer[i - 1]) return false;
    }
    return true;
}

void vAccelerometerTask(void* pvParameters)
{
    // Avoid allocating dynamic memory from tasks
    BinarySemaphore sample_sem;
    ready_to_sample = &sample_sem;

    uint8_t sample_buffer[BUFFER_WINDOW_SIZE];

    I2C i2c(I2C_BUS_FREQUENCY);

    Accelerometer accelerometer(&i2c);
    accelerometer.enable();

	TimerHandle_t accelerometer_sample_timer = xTimerCreate(
		SAMPLE_TIMER_TAG,
		pdMS_TO_TICKS(SAMPLE_TIMER_MS),
		pdTRUE,
		(void*) SAMPLE_TIMER_ID,
		sample_cube_side
	);
	xTimerStart(accelerometer_sample_timer, portMAX_DELAY);
    
    while (true)
    {   
        // Take BUFFER_WINDOW_SIZE samples before making a decision whether side changed
        for (int i = 0; i < BUFFER_WINDOW_SIZE; i++) {
            sem_sample.take();
            uint8_t side = accelerometer.get_side();
            sample_buffer[i] = side;
        }

        if (all_equal(sample_buffer, BUFFER_WINDOW_SIZE)) {
            // Give new side
        };
    }
}
