#include <CircBuffer.h>

CircBuffer::CircBuffer(int capacity) {
	buffer_size = 0;
	buffer_head = 0;
	max_capacity = capacity;
	data = new char[buffer_size];
}

int CircBuffer::size() {
	return buffer_size;
}

int CircBuffer::is_full_capacity() {
	return buffer_size == max_capacity;
}

char CircBuffer::peek() {
	return data[buffer_head];
}

char CircBuffer::pop() {
	char retval = data[buffer_head];
	buffer_size--;
	increment_buffer_head();
	return retval;
}

char CircBuffer::get(int index) {
	int buffer_index = (index + buffer_head) % max_capacity;
	return data[buffer_index];
}

void CircBuffer::put(char c) {
	data[buffer_head] = c;
	increment_buffer_head();
	if (buffer_size < max_capacity) buffer_size++;
}

void CircBuffer::increment_buffer_head() {
	int index = (buffer_head + 1) % max_capacity;
	buffer_head = index;
}