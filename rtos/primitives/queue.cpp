#include "freertos/FreeRTOS.h"
#include "queue.hpp"

/**
 * @brief FreeRTOS QueueWrapper wrapper class
 */
QueueWrapper::QueueWrapper(uint16_t length, size_t element_size)
{
	queue = xQueueCreate(length, element_size);
}

/**
 * @brief Send item to the back of the QueueWrapper
 * @param item_ptr Pointer to item to be inserted
 * @param delay    Waiting time for places to free up
 * @return         True on success
 */
bool QueueWrapper::send_back(const void* item_ptr, TickType_t delay)
{
	return xQueueSendToBack(queue, item_ptr, delay) == pdTRUE;
}

/**
 * @brief Send item to the front of the QueueWrapper
 * @param item_ptr Pointer to item to be inserted
 * @param delay    Waiting time for places to free up
 * @return         True on success
 */
bool QueueWrapper::send_front(const void* item_ptr, TickType_t delay)
{
	return xQueueSendToFront(queue, item_ptr, delay) == pdTRUE;
}

/**
 * @brief Send item to the back of the QueueWrapper from isr
 * @param item_ptr            Pointer to item to be inserted
 * @param higer_priority_flag Higher priority flag
 * @return                    True on success
 */
bool QueueWrapper::send_back_isr(const void* item_ptr, BaseType_t& higher_priority_flag)
{
	return xQueueSendToBackFromISR(queue, item_ptr, &higher_priority_flag) == pdPASS;
}

/**
 * @brief Pop last item from the QueueWrapper
 * @param item_ptr Pointer to buffer to save the item
 * @param delay    Waiting time for item to appear
 * @return         True on success
 */
bool QueueWrapper::consume(void* buffer, TickType_t delay)
{
	return xQueueReceive(queue, buffer, delay) == pdTRUE;
}

void QueueWrapper::add_to_registry(const char* tracking_name)
{
    vQueueAddToRegistry(queue, tracking_name);
}
