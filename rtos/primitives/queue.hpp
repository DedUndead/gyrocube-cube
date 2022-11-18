#ifndef QUEUE_HPP
#define QUEUE_HPP


#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class QueueWrapper {
public:
	QueueWrapper(uint16_t length, size_t element_size);
	bool send_back(const void* item_ptr, TickType_t delay = portMAX_DELAY);
	bool send_front(const void* item_ptr, TickType_t delay = portMAX_DELAY);
	bool send_back_isr(const void* item_ptr, BaseType_t& higher_priority_flag);
	bool consume(void* buffer, TickType_t delay = portMAX_DELAY);
	void add_to_registry(const char* tracking_name);
private:
	QueueHandle_t queue;
};


#endif