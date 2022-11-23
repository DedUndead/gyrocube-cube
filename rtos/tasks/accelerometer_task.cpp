#include "accelerometer_task.hpp"
#include "i2c.hpp"
#include "accelerometer.hpp"
#include "shared_resources.hpp"

#include "freertos/timers.h"


static BinarySemaphore* ready_to_sample;

/* Callback for software timer responsible for sampling */
static void sample_cube_side(TimerHandle_t xTimer)
{
    if (ready_to_sample != nullptr) { 
	    ready_to_sample.give();
    }
}

static bool all_equal(const ring_buffer<uint8_t>& buffer)
{
    uint8_t raw_buffer = buffer.get_raw_buffer_pointer();
    for (int i = 1; i < buffer.capacity(); i++) {
        if (raw_buffer[i] != raw_buffer[i - 1]) return false;
    }

    return true;
}

/*
 * @brief Accelerometer task
 * Samples current cube side with SAMPLE_TIMER_MS period
 * After three successive samples are received, the side update message is sent
 * It uses circular buffer for the most efficiency
 */
void v_accelerometer_task(void* pvParameters)
{
    // Avoid allocating dynamic memory from tasks
    BinarySemaphore sample_sem;
    ready_to_sample = &sample_sem;

    ring_buffer<uint8_t> buffer(BUFFER_WINDOW_SIZE);

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
        sem_sample.take();

        uint8_t side = accelerometer.get_side();
        buffer.put(side);

        // Cube should be stable for BUFFER_WINDOW_SIZE * SAMPLE_TIMER_MS ms to signal change
        if (buffer.full() && all_equal(buffer)) {
            accelerometer_side_queue->push_back(sample_buffer[0]);
        };
    }
}
