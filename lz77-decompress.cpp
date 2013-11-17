#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
using std::cin;

static int WINDOW_LENGTH = 150;
static int LOOKAHEAD_BUFFER_LENGTH = 30;

char c;
char c1;
char c2;
char dictionary[150];
int dictionary_head = 0;
std::vector<char> data;

void process_terminal(char terminal) {
	//printf("In process_terminal(%#10x)", terminal);
	data.push_back(terminal);
	dictionary[dictionary_head] = terminal;
	dictionary_head++;
	if (dictionary_head >= WINDOW_LENGTH) dictionary_head -= WINDOW_LENGTH;
	printf("Processing terminal: %c\n", terminal);
	//printf("%c", terminal);
}

void process_nonterminal(int offset, char length) {
	//printf("In process_nonterminal(%#10x,%#10x)",offset, length);
	int i;
	char current_literal;
	printf("Processing nonterminal: offset = %i, length = %i, dictionary = [", offset, length);
	for (i=dictionary_head; i<dictionary_head + WINDOW_LENGTH; i++) {
		printf("%c", dictionary[i % WINDOW_LENGTH]);
	}
	printf("];\n    Result is: [");
	for (i=0; i<length; i++) {
		current_literal = dictionary[(dictionary_head + i - offset) % WINDOW_LENGTH];
		dictionary[(dictionary_head + i) % WINDOW_LENGTH] = current_literal;
		data.push_back(current_literal);
		printf("%c", current_literal);
	}
	dictionary_head = (dictionary_head + length) % WINDOW_LENGTH;
	printf("]\n");
}

int main() {
	while(std::cin.get(c)) {
		if (c == 0) {
			if (std::cin.get(c1)) {
				//printf("(0, %c)\n", c1);
				process_terminal(c1);
			} else {
				printf("ERROR\n");
			}
		} else if (c == 1) {
			if (std::cin.get(c1) && std::cin.get(c2)) {
				int offset = ((c1 & 0x000000ff) << 4) + ((c2 & 0x000000f0) >> 4);
				char length = (c2 & 0x0f);
				process_nonterminal(offset, length);
				//printf("(1,%i,%i)\n",offset,length);
			} else {
				printf("FAILURE.\n");
			}
		} else {
			printf("Finished.\n");
		}
	}
	return 0;
}