#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include "CircBuffer.h"
#include <string.h>

using namespace std;

/*
* Implementation class for the Limpel-Ziv 77 compresion algorithm
* Assume alphabet of ASCII chars
* Uses a circular buffer to store lookahead buffer and recent dictionary
*
* @author Matt Jenny
*/

// Buffer lengths
static int WINDOW_LENGTH = 4095;
static int LOOKAHEAD_BUFFER_LENGTH = 30;

//Circular buffers
CircBuffer dict_circ_buffer(WINDOW_LENGTH); //Stores 4095 most recently read words
CircBuffer lookahead_circ_buffer(LOOKAHEAD_BUFFER_LENGTH); //Reads ahead 15 words

deque<string> punctuation;
CircBuffer wordqueue(20);

// Split on spaces
char delim = 0x20;
char STRING_TOKEN_DELIMITERS = 0x0A;

// Compression delimiters
static char TERMINAL_DELIMITER = 0x24;
static char NONTERMINAL_DELIMITER = 0x23;

// Command-line flags
bool verbose = false;
bool timing = false;
bool finished = false;

// Return info about a dictionary match
typedef struct {
	int is_match;
	int position;
	char length;
} Lz_match;

/**
* Find and return the maximum-length array of strings in the lookahead buffer that has a match in the dictionary buffer
*/
Lz_match get_longest_match() {
	
	if (dict_circ_buffer.is_empty()) {
		Lz_match retval = {0,0,0};
		return retval;
	}

	int is_match = 0;
	int longest_match_pos = 0;
	int longest_match_length = 0;
	int local_match_length = 0;
	int i;

	// Check each unique starting index in the dictionary
	// Keep checking following indices until one fails to match
	// Return longest matched sequence
	for (i=0; i<dict_circ_buffer.size(); i++) {
		local_match_length = 0;
		while (local_match_length <= lookahead_circ_buffer.size() && dict_circ_buffer.get(i + local_match_length).compare(lookahead_circ_buffer.get(local_match_length)) == 0) { //kind of hacky; circ buff handles the modulus
			if (is_match == 0) is_match = 1;
			local_match_length++;
		}
		if (local_match_length >= longest_match_length) {
			longest_match_pos = i;
			longest_match_length = local_match_length;
		}
	}

	Lz_match best_match = {is_match, dict_circ_buffer.size() - longest_match_pos, longest_match_length};
	return best_match;
}

void readNextToken() {
	string s;
	getline(cin, s, delim);
	size_t prev=0, next=0;
	while (prev < s.length() && (next = s.find_first_of(STRING_TOKEN_DELIMITERS, prev)) != std::string::npos) {
		if (next-prev != 0) {
			wordqueue.put(s.substr(prev, next-prev));
		}
		prev = next+1;
	}
	if (prev < s.size()) {
		wordqueue.put(s.substr(prev));
	}
}

/**
* Get the next space-delimited string from stdin
*/
string getNextString() {
	string s;
	if (!wordqueue.is_empty()) {
		s = wordqueue.pop();
		return s;
	} else {
		readNextToken();
		s = wordqueue.pop();
		return s;
	}
}

/**
* Process the next multiple strings in the text
* @param steps - The number of strings to process
*/
void advance(int steps) {
	int i=0,j;
	string s;
	for(i=0; i<steps; i++) {
		dict_circ_buffer.put(lookahead_circ_buffer.peek()); //Add the matched word to our dictionary
		if (!cin.eof()) {
			s = getNextString();
			lookahead_circ_buffer.put(s); // Read the next string and add it to the lookahead buffer
		} else { 
			lookahead_circ_buffer.pop(); // If finished reading file, just remove the lookahead buffer tail
		}
	}
}

/**
* Main LZ-77 compression routine
*/
void compress() {
	int i=0, j;
	int steps;
	vector<char> data;
	Lz_match current_match;

	// Cheat and assume the input is at least LOOKAHEAD_BUFFER_LENGTH characters long
	// Populate the lookahead buffer
	string s;
	for (i=0; i<LOOKAHEAD_BUFFER_LENGTH; i++) {
		s = getNextString();
		lookahead_circ_buffer.put(s); // Read the next string and add it to the lookahead buffer
	}

	// now that the lookahead buffer is populated, iterate through the rest of the data
	while (!lookahead_circ_buffer.is_empty()) {

		current_match = get_longest_match();
		if (current_match.is_match && current_match.length > 1) {
			// Debug information
			if (verbose) {
				printf("(1,%i,%i): ", current_match.position, current_match.length);
				int j;
				for (j=0; j<current_match.length; j++) {
					printf("%s-", lookahead_circ_buffer.get(j).c_str());
				}
				printf("\n");
			}
			steps = current_match.length; // Lookahead buffer should read the number of words that were matched

			/*
			* Bitwise manipulation, 16 bits
			* Store match position (12 bits)
			* Store length (4 bits)
			*/
			char temp1 = (char) ((current_match.position & 0x00000ff0) >> 4); // Bits 4-11 of match position
			char temp2 = (char) ((current_match.position & 0x0000000f) << 4) + (current_match.length & 0x0f); // Bits 0-3 of pos and 0-3 of length

			// Print data: nonterminal delimiter and then the 16-bit pattern we just created
			data.push_back(NONTERMINAL_DELIMITER);
			data.push_back(temp1);
			data.push_back(temp2);

		} else {
			// Debug information
			if (verbose) {
				printf("(0,%s)\n", lookahead_circ_buffer.peek().c_str());
			}
			steps = 1; // We want to read ahead one position

			// Print data: terminal delimiter...
			data.push_back(TERMINAL_DELIMITER);

			// And all characters in the terminal string
			string s = lookahead_circ_buffer.peek();
			for (string::iterator it = s.begin(); it != s.end(); ++it) {
				data.push_back(*it);
			}
		}
		advance(steps); //Set the number of words for the lookahead buffer to read ahead
	}

	//Print the data bytewise
	char* p = data.data();
	for (i=0; i<data.size(); i++) {
		putchar(*p);
		p++;
	}
}

int main(int argc, char **argv) {
	
	if (argc > 1) {
		if (strcmp("-v", argv[1]) == 0) {
			verbose = true;
		} else if (strcmp("-t", argv[1]) == 0) {
			timing = true;
		}
	}
	compress();
	return 0;
}
