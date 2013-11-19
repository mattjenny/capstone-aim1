class CircBuffer {
public:
	CircBuffer(int capacity);
	int size();
	bool is_empty();
	bool is_full_capacity();
	char peek();
	char pop();
	char get(int index);
	void get(int index, char* buffer, char length);
	void put(char c);
	void put(char* buffer, char length);

private:
	void increment_buffer_head();
	void increment_buffer_tail();
	int max_capacity;
	int buffer_size;
	char* data;
	int buffer_head;  //beginnning (oldest cell) of array
	int buffer_tail;  //end (newest cell) of array
};