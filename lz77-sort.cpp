#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <CircBuffer.h>

using namespace std;

static int WINDOW_LENGTH = 255;
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
list<string> lit;
unordered_map<string, unsigned int> counts;

bool compare_ignore_case(string first, string second) {
	unsigned int i=0;
	while (i<first.length() && i<second.length()) {
		if (tolower(first[i]) < tolower(second[i])) return true;
		if (tolower(first[i]) > tolower(second[i])) return false;
		i++;
	}
	return (first.length() < second.length());
}

void process_terminal(string terminal) {
	if (verbose) printf("(0,%s):  %s\n", terminal.c_str(), terminal.c_str());
	dict.put(terminal);
	counts.at(terminal)++;
}

void process_nonterminal(int offset, char length) {
	if (verbose) printf("(1,%i,%i):  ",offset, length);
	int i;
	string* decoded_str = new string[length];
	int index = dict.size() - offset;
	dict.get(index, decoded_str, length); //get the data encoded by the pointer
	dict.put(decoded_str, length); //store this data in the dictionary buffer
	for (i=0; i<length; i++) {
		counts.at(decoded_str[i])++; 
		if (verbose) printf("%s-", decoded_str[i].c_str());
	}
	if (verbose) printf("\n");
	delete [] decoded_str;
}

void lz77_sort() {
	unordered_set<string> lit_set;
	string line;
	char current_delimiter;
	istreambuf_iterator<char> eos;
	line = string(istreambuf_iterator<char> (cin), eos);

	//Find all unique literals
	current_delimiter = line.at(0);
	size_t prev = 1, pos;
	while(prev < line.length()) {
		if (current_delimiter == TERMINAL_DELIMITER) {
			pos = line.find_first_of(delim, prev);
			if (pos == string::npos) {
				pos = line.length()-1;
				lit_set.insert(line.substr(prev, line.length()-prev));
			}
			else
				lit_set.insert(line.substr(prev, pos-prev));
		} else {
			pos = prev+2;
		}
		current_delimiter = line.at(pos);
		prev = pos+1;
	}

	for (unordered_set<string>::iterator it = lit_set.begin(); it != lit_set.end(); ++it) {
		lit.push_back(*it);
		counts.insert(make_pair<string, unsigned int>(*it, 0));
	}
	lit.sort(compare_ignore_case);

	//Tally the numbers of each literal
	current_delimiter = line.at(0);
	prev = 1, pos = 0;
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

	printf("\nRESULTS:\n");
	for(list<string>::iterator lit_it = lit.begin(); lit_it != lit.end(); ++lit_it) {
		printf("(%s,%u) ",(*lit_it).c_str(),counts.at(*lit_it));
	}
	printf("\n");
}

int main(int argc, char** argv) {
	if (argc > 1 && strcmp("-v", argv[1]) == 0) {
		verbose = true;
	}
	lz77_sort();
	return 0;
}