#include <stdio.h>
#include <string.h>
#include <iostream>
using std::cin;

static int WINDOW_LENGTH = 150;
static int LOOKAHEAD_BUFFER_LENGTH = 30;

char c;
char c1;
char c2;

int main() {
	while(std::cin.get(c)) {
		if (c == 0) {
			if (std::cin.get(c1)) {
				printf("(0, %c)\n", c1);
			} else {
				printf("ERROR\n");
			}
		} else if (c == 1) {
			if (std::cin.get(c1) && std::cin.get(c2)) {
				int offset = ((c1 & 0x000000ff) << 4) + ((c2 & 0x000000f0) >> 4);
				char length = (c2 & 0x0f);
				printf("(1,%i,%i)\n",offset,length);
			} else {
				printf("FAILURE.\n");
			}
		} else {
			printf("Finished.\n");
		}
	}
	return 0;
}