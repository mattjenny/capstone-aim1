#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <CircBuffer.h>
//using std::cin;
using namespace std;

//Assume alphabet of ASCII chars

static int WINDOW_LENGTH = 150;
static int LOOKAHEAD_BUFFER_LENGTH = 30;
static int LONGEST_MATCH = 15;
CircBuffer dict_circ_buffer(WINDOW_LENGTH);
CircBuffer lookahead_circ_buffer(LOOKAHEAD_BUFFER_LENGTH);

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

		/*	if (dict_circ_buffer.size() < WINDOW_LENGTH) {
		printf("Dictionary = [");
		int j;
		for (j=0; j<dict_circ_buffer.size(); j++) {
			printf("%c", dict_circ_buffer.get(j));
		}
		printf("\nComparing %c and %c\n", dict_circ_buffer.get(i + local_match_length), lookahead_circ_buffer.get(local_match_length));
		}*/

	for (i=0; i<dict_circ_buffer.size(); i++) {
		local_match_length = 0;
		while (local_match_length <= lookahead_circ_buffer.size() && local_match_length < LONGEST_MATCH && dict_circ_buffer.get(i + local_match_length) == lookahead_circ_buffer.get(local_match_length)) { //kind of hacky; circ buff handles the modulus
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

void advance(int steps) {
	int i;
	char c;
	for (i=0; i<steps; i++) {
		dict_circ_buffer.put(lookahead_circ_buffer.peek());
		if (lookahead_circ_buffer.is_full_capacity() && cin.get(c)) {
			lookahead_circ_buffer.put(c);
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
	char c;
	for (i = 0; i<LOOKAHEAD_BUFFER_LENGTH; i++) {
		cin.get(c);
		lookahead_circ_buffer.put(c);
	}

	// now that the lookahead buffer is populated, iterate through the rest of the data
	while (!lookahead_circ_buffer.is_empty()) {

		current_match = get_longest_match();
		if (current_match.is_match) {
			if (verbose) {
				printf("(1,%i,%i): ", current_match.position, current_match.length);
				int j;
				for (j=0; j<current_match.length; j++) {
					printf("%c", lookahead_circ_buffer.get(j));
				}
				printf("\n");
			}
			steps = current_match.length;

			char temp1 = (char) ((current_match.position & 0x00000ff0) >> 4);
			char temp2 = (char) ((current_match.position & 0x0000000f) << 4) + (current_match.length & 0x0f);

			data.push_back(1);
			data.push_back(temp1);
			data.push_back(temp2);

		} else {
			if (verbose) {
				printf("(0,%c)\n", lookahead_circ_buffer.peek());
			}
			steps = 1;
			data.push_back(0);
			data.push_back(lookahead_circ_buffer.peek());
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