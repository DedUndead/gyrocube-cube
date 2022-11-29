#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP


#include <stddef.h>

#define RING_BUFFER_MAX_SIZE 50

/* 
 * Circular buffer implementation. 
 * The most effective way of handling stream data, e.g., accelerometer sampling
 * Partially adopted from: https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/
 */
template <class T> class ring_buffer {
public:
	explicit ring_buffer(size_t size) : max_size(size) { }
	void put(T item);
	T get(bool consume = false);
    T* get_raw_buffer_pointer();
	void reset();
	bool empty() const;
	bool full() const;
	size_t capacity() const;
	size_t size() const;
private:
	T buffer[RING_BUFFER_MAX_SIZE];
	size_t head = 0;
	size_t tail = 0;
	const size_t max_size;
	bool full_ = false;
};


#endif