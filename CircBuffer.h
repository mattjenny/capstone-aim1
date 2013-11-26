#include <string>

using namespace std;

class CircBuffer {
public:
	CircBuffer(int capacity);
	int size();
	bool is_empty();
	bool is_full_capacity();
	string peek();
	string pop();
	string get(int index);
	void get(int index, string* buffer, char length);
	void put(string c);
	void put(string* buffer, char length);

private:
	void increment_buffer_head();
	void increment_buffer_tail();
	int max_capacity;
	int buffer_size;
	string* data;
	int buffer_head;  //beginnning (oldest cell) of array
	int buffer_tail;  //end (newest cell) of array
};