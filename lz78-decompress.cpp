#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <deque>
#include <vector>

using namespace std;

struct Dictionary_Pair {
	int prefix_index;
	string literal;
};

deque<Dictionary_Pair> dictionary;
vector<string> data;
bool verbose = false;

void process_index(int index) {
	if (index == 0) return;
	else if (index > dictionary.size()) {
		printf("Index out of bounds.");
		return;
	}
	else {
		Dictionary_Pair pair = dictionary.at(index);
		process_index(pair.prefix_index);
		data.push_back(pair.literal);
	}
}

void process_pair(int index, string literal) {
	if (index > dictionary.size()) {
		printf("Index out of bounds.");
		return;
	}
	process_index(index);
	data.push_back(literal);
	Dictionary_Pair current_pair = {index, literal};
	dictionary.push_back(current_pair);
}

void decompress() {
	// Dictionary(0) is empty
	Dictionary_Pair root_pair = {0, ""};
	dictionary.push_back(root_pair);

	// Read cin into a string
	string s;
	cin >> s;

	// Iterate once for each (index, literal) pair.  prev keeps track of the current index in string s
	int prev = 0;
	while (prev < s.length()) {
		string literal;
		int index = (((int) s.at(prev)) << 16) + (((int) s.at(prev+1)) << 8) + ((int) s.at(prev+2)); //Re-create int.  Endianness makes this tough to do cleanly.
		char length = s.at(prev+3);
		if (length > 0) literal = s.substr(prev+4, length);
		else literal = "";
		if (verbose) printf("(%i,%s)\n", index, literal.c_str());
		process_pair(index, literal);
		prev += (length + 4); //step forward to the start of the next pair
	}

	// Print the result
	int i;
	for (i=0; i<data.size(); i++) {
		printf("%s ", data.at(i).c_str());
	}
	printf("\n");
}

int main(int argc, char** argv) {
	if (argc > 1 && strcmp("-v", argv[1]) == 0) {
		verbose = true;
	}
	decompress();
	return 0;
}