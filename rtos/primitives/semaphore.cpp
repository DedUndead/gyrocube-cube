#include "freertos/FreeRTOS.h"

#include "semaphore.hpp"

BinarySemaphore::BinarySemaphore()
{
	sem = xSemaphoreCreateBinary();
}

BinarySemaphore::~BinarySemaphore() {}

bool BinarySemaphore::take()
{
	return xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE;
}

void BinarySemaphore::give()
{
	xSemaphoreGive(sem);
}



