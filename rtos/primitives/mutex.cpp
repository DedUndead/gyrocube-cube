#include "mutex.hpp"


Fmutex::Fmutex() {
	mutex = xSemaphoreCreateMutex();
}

Fmutex::~Fmutex() {
	// TODO Auto-generated destructor stub
	vSemaphoreDelete(mutex);
}

void Fmutex::lock()
{
	xSemaphoreTake(mutex, portMAX_DELAY);
}

void Fmutex::unlock()
{
	xSemaphoreGive(mutex);
}
