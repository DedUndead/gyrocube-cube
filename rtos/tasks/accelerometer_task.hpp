#define I2C_BUS_FREQUENCY 250 * 1000

#define SAMPLE_TIMER_TAG "ACCSAMPLE"
#define SAMPLE_TIMER_ID  0
#define SAMPLE_TIMER_MS  100

#define BUFFER_WINDOW_SIZE 3
#define V_ACCELEROMETER_TASK_LOG_TAG "vACCEL"

void v_accelerometer_task(void* pvParameters);
