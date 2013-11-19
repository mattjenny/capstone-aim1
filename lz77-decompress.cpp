#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <CircBuffer.h>
using std::cin;

static int WINDOW_LENGTH = 150;
static int LOOKAHEAD_BUFFER_LENGTH = 30;

char c;
char c1;
char c2;
bool verbose = false;
CircBuffer dict(WINDOW_LENGTH);
std::vector<char> data;

void process_terminal(char terminal) {
	if (verbose) printf("(0,%c):  %c\n", terminal, terminal);
	data.push_back(terminal);
	dict.put(terminal);
}

void process_nonterminal(int offset, char length) {
	int i;
	char* decoded_str = new char[length];
	int index = dict.size() - offset;
	dict.get(index, decoded_str, length); //get the data encoded by the pointer
	dict.put(decoded_str, length); //store this data in the dictionary buffer
	if (verbose) printf("(1,%i,%i):  ",offset, length);
	for (i=0; i<length; i++) {
		data.push_back(decoded_str[i]); 
		if (verbose) printf("%c", decoded_str[i]);
	}
	if (verbose) printf("\n");
	delete [] decoded_str;
}

int main(int argc, char** argv) {
	if (argc > 1 && strcmp("-v", argv[1]) == 0) {
		verbose = true;
	}
	while(std::cin.get(c)) {
		// Test for terminal sequence
		if (c == 0) {
			// Next char contains the ASCII representation of the terminal
			if (std::cin.get(c1)) {
				process_terminal(c1);
			} else {
				printf("ERROR\n");
			}
		// Test for nonterminal sequence
		} else if (c == 1) {
			//Next two chars contain offset (12 bits) and length (4 bits) of the match
			if (std::cin.get(c1) && std::cin.get(c2)) {
				int offset = ((c1 & 0x000000ff) << 4) + ((c2 & 0x000000f0) >> 4);
				char length = (c2 & 0x0f);
				process_nonterminal(offset, length);
			} else {
				printf("ERROR\n");
			}
		// End-of-file or other
		} else {}
	}

	// Print the decompressed data to stdout
	printf("%s\n", data.data());

	return 0;
}