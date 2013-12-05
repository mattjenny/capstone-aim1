#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <CircBuffer.h>

using namespace std;

static int WINDOW_LENGTH = 2047;
static int LOOKAHEAD_BUFFER_LENGTH = 15;
static char TERMINAL_DELIMITER = 0x24;
static char NONTERMINAL_DELIMITER = 0x23;

char c;
char c1;
char c2;
char delim[2] = {TERMINAL_DELIMITER, NONTERMINAL_DELIMITER};
bool verbose = false;
CircBuffer dict(WINDOW_LENGTH);
std::vector<string> data;

void process_terminal(string terminal) {
	if (verbose) printf("(0,%s):  %s\n", terminal.c_str(), terminal.c_str());
	data.push_back(terminal);
	dict.put(terminal);
}

void process_nonterminal(int offset, char length) {
	if (verbose) printf("(1,%i,%i):  ",offset, length);
	int i;
	string* decoded_str = new string[length];
	int index = dict.size() - offset;
	dict.get(index, decoded_str, length); //get the data encoded by the pointer
	dict.put(decoded_str, length); //store this data in the dictionary buffer
	for (i=0; i<length; i++) {
		data.push_back(decoded_str[i]); 
		if (verbose) printf("%s-", decoded_str[i].c_str());
	}
	if (verbose) printf("\n");
	delete [] decoded_str;
}

void decompress() {
	string line;
	char current_delimiter;
	istreambuf_iterator<char> eos;
	line = string(istreambuf_iterator<char> (cin), eos);
	current_delimiter = line.at(0);
	size_t prev = 1, pos;
	while(prev < line.length()) {
		if (current_delimiter == TERMINAL_DELIMITER) {
			pos = line.find_first_of(delim, prev);
			if (pos == string::npos) {
				pos = line.length()-1;
				process_terminal(line.substr(prev, line.length()-prev));
			}
			else
				process_terminal(line.substr(prev, pos-prev));
		} else if (current_delimiter == NONTERMINAL_DELIMITER) {
			int offset = ((line.at(prev) & 0x000000ff) << 4) + ((line.at(prev+1) & 0x000000f0) >> 4);
			char length = (line.at(prev+1) & 0x0f);
			pos = prev+2;
			process_nonterminal(offset, length);
		}
		current_delimiter = line.at(pos);
		prev = pos+1;
	}

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
