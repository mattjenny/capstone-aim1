#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <deque>
#include <vector>

using namespace std;

struct Dictionary_Pair {
	unsigned int prefix_index;
	char literal;
};

deque<Dictionary_Pair> dictionary;
vector<char> data;
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
		printf("%c", pair.literal);
	}
}

void process_pair(int index, char literal) {
	if (index > dictionary.size()) {
		printf("Index out of bounds.");
		return;
	}
	process_index(index);
	data.push_back(literal);
	if (verbose) printf("%c", literal);
	Dictionary_Pair current_pair = {index, literal};
	dictionary.push_back(current_pair);
}

void decompress() {
	// Dictionary(0) is empty
	Dictionary_Pair root_pair = {0, 0x00};
	dictionary.push_back(root_pair);

	char c, literal;
	unsigned int i1, i2, i3, index;
	while ((c = getchar()) != EOF) {
		i1 = (unsigned int) c;
		i2 = (unsigned int) getchar();
		i3 = (unsigned int) getchar();
		index = ((i1 << 16) & 0x00ff0000) + ((i2 << 8) & 0x0000ff00) + i3;
		literal = getchar();
		if (verbose) printf("(i1 = %u, i2 = %u, i3 = %u) -> %u\n", i1, i2, i3, index);
		process_pair(index, literal);
		if (verbose) printf ("---(%i,%c)\n", index, literal);
	}

	// Print the result
	int i;
	for (i=0; i<data.size(); i++) {
		printf("%c", data.at(i));
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