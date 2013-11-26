#include <CircBuffer.h>
#include <stdio.h>
#include <string>

CircBuffer::CircBuffer(int capacity) {
	buffer_size = 0;
	buffer_head = 0;
	buffer_tail = 0;
	max_capacity = capacity;
	data = new string[max_capacity];
}

int CircBuffer::size() {
	return buffer_size;
}

bool CircBuffer::is_empty() {
	return buffer_size == 0;
}

bool CircBuffer::is_full_capacity() {
	return buffer_size == max_capacity;
}

string CircBuffer::peek() {
	return data[buffer_tail];
}

string CircBuffer::pop() {
	string retval = data[buffer_tail];
	increment_buffer_tail();
	buffer_size--;
	return retval;
}

string CircBuffer::get(int index) {
	if(is_full_capacity()) return data[(buffer_head + index) % max_capacity];
	else {
		return data[(buffer_tail + (index % buffer_size)) % max_capacity];
	}
}

// buffer is a char array of length "length" that will be populated by get()
void CircBuffer::get(int index, string* buffer, char length) { 
	int i;
	for (i=0; i<length; i++) {
		buffer[i] = get(i + index);
	}
}

void CircBuffer::put(string c) {
	data[buffer_head] = c;
	increment_buffer_head();
	if (is_full_capacity()) increment_buffer_tail();
	if (buffer_size < max_capacity) buffer_size++;
}

void CircBuffer::put(string* buffer, char length) {
	int i;
	for(i=0; i<length; i++) {
		put(buffer[i]);
	}
}

void CircBuffer::increment_buffer_head() {
	int index = (buffer_head + 1) % max_capacity;
	buffer_head = index;
}

void CircBuffer::increment_buffer_tail() {
	buffer_tail = (buffer_tail + 1) % max_capacity;
}