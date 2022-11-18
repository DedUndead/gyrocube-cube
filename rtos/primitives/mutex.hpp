#ifndef MUTEX_HPP
#define MUTEX_HPP


#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class Fmutex {
public:
	Fmutex();
	virtual ~Fmutex();
	void lock();
	void unlock();
private:
	SemaphoreHandle_t mutex;
};


#endif
