class CircBuffer {
public:
	CircBuffer(int capacity);
	int size();
	int is_full_capacity();
	char peek();
	char pop();
	char get(int index);
	void put(char c);

private:
	void increment_buffer_head();
	int max_capacity;
	int buffer_size;
	char* data;
	int buffer_head;  //beginnning (oldest cell) of array
};