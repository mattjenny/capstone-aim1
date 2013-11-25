#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include <CircBuffer.h>
//using std::cin;
using namespace std;

//Assume alphabet of ASCII chars

static int WINDOW_LENGTH = 150;
static int LOOKAHEAD_BUFFER_LENGTH = 15;
CircBuffer dict_circ_buffer(WINDOW_LENGTH);
CircBuffer lookahead_circ_buffer(LOOKAHEAD_BUFFER_LENGTH);
deque<string> punctuation;
//char punct[11] = ";:,.!?()\"\t\n";
char punct[11] = {0x09, 0x0a, 0x21, 0x22, 0x28, 0x29, 0x2c, 0x2e, 0x3a, 0x3b, 0x3f};
char delim = 0x20;

typedef struct {
	int is_match;
	int position;
	char length;
} Lz_match;

typedef struct {
	int dictionary_head;
	int buffer_head;
	int dictionary_length;
	int buffer_length;
} Advance_data;

int positive_mod(int x, int y) {
	return (x%y + y)%y;
}

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

string getNextString() {
	if(punctuation.empty()) {
		string s;
		size_t prev=0, pos;
		getline(cin,s,delim);
		while((pos = s.find_first_of(punct, prev)) != string::npos) {
			if (pos-prev > 1) 
				punctuation.push_back(s.substr(prev, pos-prev));
			punctuation.push_back(s.substr(pos, 1));
			prev = pos + 1;
		}
		if (prev < s.length()) {
			punctuation.push_back(s.substr(prev, s.length()-prev));
		}
	}
	string retval = punctuation.front();
	punctuation.pop_front();
	return retval;
}

void advance(int steps) {
	int i;
	string s;
	for (i=0; i<steps; i++) {
		dict_circ_buffer.put(lookahead_circ_buffer.peek());
		if (!cin.eof() || !punctuation.empty()) {
			//getline(cin,s,delim);
			s = getNextString();
			//printf("GETTING NEXT STRING: %s\n", s.c_str());
			lookahead_circ_buffer.put(s);
		} else {
			lookahead_circ_buffer.pop();
		}
	}
}

int main(int argc, char **argv) {

	int i;
	int steps;
	vector<char> data;
	bool verbose = false;
	if (argc > 1 && strcmp("-v", argv[1]) == 0) {
		verbose = true;
	}

	Lz_match current_match;

	//Cheat and assume the input is at least LOOKAHEAD_BUFFER_LENGTH characters long
	string s;
	char delim = 0x20;
	for (i = 0; i<LOOKAHEAD_BUFFER_LENGTH; i++) {
		//getline(cin,s,delim);
		s = getNextString();
		//printf("GETTING NEXT STRING: %s\n", s.c_str());
		lookahead_circ_buffer.put(s);
	}

	// now that the lookahead buffer is populated, iterate through the rest of the data
	while (!lookahead_circ_buffer.is_empty()) {

		current_match = get_longest_match();
		if (current_match.is_match && current_match.length > 1) {
			if (verbose) {
				printf("(1,%i,%i): ", current_match.position, current_match.length);
				int j;
				for (j=0; j<current_match.length; j++) {
					printf("%s-", lookahead_circ_buffer.get(j).c_str());
				}
				printf("\n");
			}
			steps = current_match.length;

			char temp1 = (char) ((current_match.position & 0x00000ff0) >> 4);
			char temp2 = (char) ((current_match.position & 0x0000000f) << 4) + (current_match.length & 0x0f);

			data.push_back(0x23);
			data.push_back(temp1);
			data.push_back(temp2);

		} else {
			if (verbose) {
				printf("(0,%s)\n", lookahead_circ_buffer.peek().c_str());
			}
			steps = 1;
			data.push_back(0x24);
			string s = lookahead_circ_buffer.peek();
			for (string::iterator it = s.begin(); it != s.end(); ++it) {
				data.push_back(*it);
			}
			//data.push_back(lookahead_circ_buffer.peek());
		}
		advance(steps);
	}

	//Print the data bytewise
	char* p = data.data();
	for (i=0; i<data.size(); i++) {
		putchar(*p);
		p++;
	}
	printf("\n", data.data());

	return 0;
}