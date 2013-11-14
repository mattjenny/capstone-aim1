#include <stdio.h>
#include <string.h>
#include <iostream>
using std::cin;

//Assume alphabet of ASCII chars

static int WINDOW_LENGTH = 30;
static int LOOKAHEAD_BUFFER_LENGTH = 10;

typedef struct {
	int is_match;
	int position;
	int length;
} Lz_match;

int positive_mod(int x, int y) {
	return (x%y + y)%y;
}

Lz_match get_longest_match(char* dictionary, int dict_start_pos, int dict_length, char* lookahead_buffer, int buffer_start_pos, int buffer_length) {
	int is_match = 0;
	int longest_match_pos = -1;
	int longest_match_length = 0;
	int i;

	for (i=0; i<dict_length; i++) {
		if (dictionary[i] == lookahead_buffer[0]) {
			is_match = 1;
			int local_match_length = 1;
			while (local_match_length < buffer_length && dictionary[(i+local_match_length) % dict_length] == lookahead_buffer[(local_match_length + buffer_start_pos) % buffer_length]) {
				local_match_length++;
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

int main(int argc, char **argv) {

	int i;
	int dictionary_length = 0;
	int dictionary_head = 0;
	int buffer_head = 0;
	char dictionary[WINDOW_LENGTH];
	char buffer[LOOKAHEAD_BUFFER_LENGTH];
	int buffer_length = LOOKAHEAD_BUFFER_LENGTH;
	Lz_match current_match;
	char c;

	//Cheat and assume the input is at least LOOKAHEAD_BUFFER_LENGTH characters long
	for (i = 0; i<LOOKAHEAD_BUFFER_LENGTH; i++) {
		std::cin.get(c);
		buffer[i] = c;
		printf("On character: %c\n",c);
	}

	// now that the lookahead buffer is populated, iterate through the rest of the data
	while (std::cin.get(c)) {
		printf("On character: %c\n",c);
		current_match = get_longest_match(dictionary, dictionary_length, WINDOW_LENGTH, buffer, buffer_head, LOOKAHEAD_BUFFER_LENGTH);
		if (current_match.is_match) {
			printf("(1,%i,%i)", current_match.position, current_match.length);
			buffer[buffer_head] = c;
			buffer_head++;
			if (buffer_head >= LOOKAHEAD_BUFFER_LENGTH) buffer_head -= LOOKAHEAD_BUFFER_LENGTH;
			dictionary[dictionary_head] = c;
			dictionary_head++;
			if (dictionary_head >= WINDOW_LENGTH) dictionary_head -= WINDOW_LENGTH;
			if (dictionary_length < WINDOW_LENGTH) dictionary_length++;	
		} else {
			printf("(0,%c) ", c);
			buffer[buffer_head] = c;
			buffer_head++;
			if (buffer_head >= LOOKAHEAD_BUFFER_LENGTH) buffer_head -= LOOKAHEAD_BUFFER_LENGTH;
			dictionary[dictionary_head] = c;
			dictionary_head++;
			if (dictionary_head >= WINDOW_LENGTH) dictionary_head -= WINDOW_LENGTH;
			if (dictionary_length < WINDOW_LENGTH) dictionary_length++;
		}
	}
	/*Lz_match match = get_longest_match("dcdbabc", 2, 7, "abca", 0, 4);
	printf("\nis_match = %i\n", match.is_match);
	printf("match position = %i\n", match.position);
	printf("match length = %i\n", match.length);*/
	printf("\n");
	/*
	while (fgets(buffer, LOOKAHEAD_BUFFER_LENGTH, stdin)) {
		printf("%s", buffer);
		Lz_match current_match = get_longest_match(dictionary, dictionary_length, buffer, buffer_length);
	}
	printf("\n");
	*/
	/*
	if (argc != 2) {
		printf("This function takes in one parameter; %i were supplied.\n.", argc - 1);
		return 0;
	}
	char *s = argv[1];
	printf("%s\n", s);*/
	return 0;
}