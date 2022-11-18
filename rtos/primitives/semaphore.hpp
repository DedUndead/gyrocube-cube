#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP


#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class BinarySemaphore {
public:
	BinarySemaphore();
	virtual ~BinarySemaphore();
	bool take();
	void give();
private:
	SemaphoreHandle_t sem;
};


#endif
