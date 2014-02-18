#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include "CircBuffer.h"

using namespace std;

static int WINDOW_LENGTH = 4095;
static int LOOKAHEAD_BUFFER_LENGTH = 15;
static char TERMINAL_DELIMITER = 0xB1;
static char NONTERMINAL_DELIMITER = 0xB2;

char c;
char c1;
char c2;
char delim[2] = {TERMINAL_DELIMITER, NONTERMINAL_DELIMITER};
bool verbose = false;
CircBuffer dict(WINDOW_LENGTH);
std::vector<char> data;

// Read a terminal character and add it to the decompressed data
void process_terminal(char terminal) {
	if (verbose) printf("(0,%c):  %c\n", terminal, terminal);
	data.push_back(terminal);
	dict.put(terminal); // Add to lookback dictionary
}

// Look up the referenced character(s) in the dictionary
void process_nonterminal(int offset, char length) {
	if (verbose) printf("(1,%i,%i):  ",offset, length);
	int i;
	char* decoded_str = new char[length]; // Make space for the new string(s)
	int index = dict.size() - offset;
	dict.get(index, decoded_str, length); //get the data encoded by the pointer
	dict.put(decoded_str, length); //store this data in the dictionary buffer
	for (i=0; i<length; i++) {
		data.push_back(decoded_str[i]); // Store string(s)
		if (verbose) printf("%c-", decoded_str[i]);
	}
	if (verbose) printf("\n");
	delete [] decoded_str;
}

// Main decompression routine
void decompress() {
	/*
	string line; // String to read cin into
	char current_delimiter; // Used to point to the beginning of the current terminal or nonterminal
	istreambuf_iterator<char> eos; // Iterator used to read cin into a string
	line = string(istreambuf_iterator<char> (cin), eos); // Read cin into a string.  There's a more efficient way to do this I'm sure...
	current_delimiter = line.at(0); //Start at the beginning
	size_t prev = 1, pos; //scan forward from prev to find next delimiter symbol

	while(prev < line.length()) { // Iterate through all terminals and nonterminals
		if (current_delimiter == TERMINAL_DELIMITER) {
			pos = line.find_first_of(delim, prev); // Scan for next delimiter to determine how long this terminal is
			if (pos == string::npos) { // Check if this is the end of the string
				pos = line.length()-1;
				process_terminal(line.substr(prev, line.length()-prev));
			}
			else
				process_terminal(line.substr(prev, pos-prev)); // Do useful things
		} else if (current_delimiter == NONTERMINAL_DELIMITER) {
			int offset = ((line.at(prev) & 0x000000ff) << 4) + ((line.at(prev+1) & 0x000000f0) >> 4); //Bitwise manipulations to get offset
			char length = (line.at(prev+1) & 0x0f); // Bitwise manipulations to get length
			pos = prev+2; // We know that this nonterminal is exactly 16 bytes
			process_nonterminal(offset, length); // Do useful things
		}
		current_delimiter = line.at(pos); 
		prev = pos+1;
	}
	*/
	int offset;
	char c, terminal, n1, n2, length;
	while ((c = getchar()) != EOF) {
		if (c == TERMINAL_DELIMITER) {
			terminal = getchar();
			process_terminal(terminal);
		} else if (c == NONTERMINAL_DELIMITER) {
			n1 = getchar();
			n2 = getchar();
			offset = ((n1 & 0x000000ff) << 4) + ((n2 & 0x000000f0) >> 4); //Bitwise manipulations to get offset
			length = (n2 & 0x0f); // Bitwise manipulations to get length
			process_nonterminal(offset, length);
		}
	}

	// Print decompressed data
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
