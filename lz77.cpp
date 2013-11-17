#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
using std::cin;

//Assume alphabet of ASCII chars

static int WINDOW_LENGTH = 150;
static int LOOKAHEAD_BUFFER_LENGTH = 30;

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

Lz_match get_longest_match(char* dictionary, int dict_start_pos, int dict_length, char* lookahead_buffer, int buffer_start_pos, int buffer_length) {
	
	if (dict_length == 0) {
		Lz_match retval = {0,0,0};
		return retval;
	}

	int is_match = 0;
	int longest_match_pos = 0;
	int longest_match_length = 0;
	int i;

	//int j;
			//printf("BUFFER: ");
			//for (j=0; j<buffer_length; j++) {
			//	printf("%c", lookahead_buffer[(j + buffer_start_pos) % LOOKAHEAD_BUFFER_LENGTH]);
			//}
			//printf("; head at index %i; buffer_length = %i\n", buffer_start_pos, buffer_length);

	for (i=0; i<dict_length; i++) {
		//printf("In match function; comparing %c with %c\n", dictionary[i], lookahead_buffer[buffer_start_pos]);
		if (dictionary[i] == lookahead_buffer[buffer_start_pos]) {
			is_match = 1;
			int local_match_length = 1;
			//printf("   now comparing %c to %c\n", dictionary[(i+local_match_length) % dict_length], lookahead_buffer[(local_match_length + buffer_start_pos) % LOOKAHEAD_BUFFER_LENGTH]);
			while (local_match_length < buffer_length && dictionary[(i+local_match_length) % dict_length] == lookahead_buffer[(local_match_length + buffer_start_pos) % LOOKAHEAD_BUFFER_LENGTH]) {
				local_match_length++;
				//printf("i = %i, local_match_length = %i, dict_length = %i\n", i, local_match_length, dict_length);
				//printf("   now comparing %c to %c\n", dictionary[(i+local_match_length) % dict_length], lookahead_buffer[(local_match_length + buffer_start_pos) % LOOKAHEAD_BUFFER_LENGTH]);
			}
			if (local_match_length >= longest_match_length) {
				longest_match_pos = i;
				longest_match_length = local_match_length;
			}
		}
	}

	Lz_match best_match = {is_match, positive_mod((dict_start_pos - longest_match_pos), dict_length), longest_match_length};
	return best_match;
}

Advance_data advance(char* dictionary, int dictionary_head, int dictionary_length, char* buffer, int buffer_head, int buffer_length, int steps) {
	int i;
	char c;
	for (i=0; i<steps; i++) {
		dictionary[dictionary_head] = buffer[buffer_head];
		dictionary_head++;
		if (dictionary_length < WINDOW_LENGTH) dictionary_length = dictionary_head;
		if (dictionary_head > WINDOW_LENGTH) dictionary_head -= WINDOW_LENGTH;
		if (buffer_length == LOOKAHEAD_BUFFER_LENGTH && std::cin.get(c)) {
			buffer[buffer_head] = c;
			buffer_head++;
			if (buffer_head > LOOKAHEAD_BUFFER_LENGTH) buffer_head -= LOOKAHEAD_BUFFER_LENGTH;
		} else {
			buffer_length--;
			buffer_head++;
			if (buffer_head > LOOKAHEAD_BUFFER_LENGTH) buffer_head -= LOOKAHEAD_BUFFER_LENGTH;
		}
	}
	Advance_data retval = {dictionary_head, buffer_head, dictionary_length, buffer_length};
	return retval;
}

int main(int argc, char **argv) {

	int i;
	int steps;
	//int dictionary_length = 0;
	int dictionary_head = 0;
	int buffer_head = 0;
	char dictionary[WINDOW_LENGTH];
	char buffer[LOOKAHEAD_BUFFER_LENGTH];
	int buffer_length = LOOKAHEAD_BUFFER_LENGTH;
	std::vector<char> data;

	Lz_match current_match;
	Advance_data advance_data = {0,0,0,LOOKAHEAD_BUFFER_LENGTH};

	//Cheat and assume the input is at least LOOKAHEAD_BUFFER_LENGTH characters long
	char c;
	for (i = 0; i<LOOKAHEAD_BUFFER_LENGTH; i++) {
		std::cin.get(c);
		buffer[i] = c;
		//printf("Buffering character: %c\n",c);
	}

	// now that the lookahead buffer is populated, iterate through the rest of the data
	while (advance_data.buffer_length > 0) {
		//printf("\n----------------------------\n");
		current_match = get_longest_match(dictionary, advance_data.dictionary_head, advance_data.dictionary_length, buffer, advance_data.buffer_head, advance_data.buffer_length);
		if (current_match.is_match) {
			/*printf("(1,%i,%i): ", current_match.position, current_match.length);
			int j;
			for (j=0; j<current_match.length; j++) {
				printf("%c", buffer[positive_mod((advance_data.buffer_head + j), LOOKAHEAD_BUFFER_LENGTH)]);
			}
			printf("\n");*/
			steps = current_match.length;

			char temp1 = (char) ((current_match.position & 0x00000ff0) >> 4);
			char temp2 = (char) ((current_match.position & 0x0000000f) << 4) + (current_match.length & 0x0f);

			data.push_back(1);
			data.push_back(temp1);
			data.push_back(temp2);
			//printf("Position = %#06x; Length = %#06x; chars are %#04x and (%#03x + %#03x) => %#04x\n", current_match.position, current_match.length, temp1, ((current_match.position & 0x0000000f) << 4), (current_match.length & 0x0f), temp2);
			//printf("data: 1 - %#06x\n", temp);

		} else {
			//printf("(0,%c)\n", buffer[advance_data.buffer_head]);
			steps = 1;
			data.push_back(0);
			data.push_back(buffer[advance_data.buffer_head % buffer_length]);
			/*printf("Printing nonterminal: %c\n", buffer[advance_data.buffer_head % buffer_length]);
			printf("    Buffer = [");
			int k;
			for(k=advance_data.buffer_head; k<advance_data.buffer_head + buffer_length; k++) {
				printf("%c", buffer[k % buffer_length]);
			}
			printf("]\n");*/
		}
		advance_data = advance(dictionary, advance_data.dictionary_head, advance_data.dictionary_length, buffer, advance_data.buffer_head, advance_data.buffer_length, steps);
	}
	/*
	printf("data: ");
	for (i=0; i<data.size(); i++) {
		printf("-%i-", data.at(i));
	}
	printf("\n");
*/
	char* p = data.data();
	for (i=0; i<data.size(); i++) {
		putchar(*p);
		p++;
	}
	printf("\n");

	return 0;
}