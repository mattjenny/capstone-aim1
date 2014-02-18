#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

unsigned int node_count = 0;
int match_length = 0;
static int MAX_MATCH_LENGTH = 255;
//char delim = 0x20;
bool verbose = false;
bool end_of_file = false;

typedef struct {
	unsigned int prefix_index;
	char literal;
} Lz_pair;

struct Dictionary_Node {

	unsigned int index;
	char literal;
	vector<Dictionary_Node*> children;

	Dictionary_Node(unsigned int index_in, char literal_in) {
		index = index_in;
		literal = literal_in;
	}

	void addChild(Dictionary_Node* child) {
		children.push_back(child);
	}

	Dictionary_Node* getChild(char c) {
		for(vector<Dictionary_Node*>::iterator it = children.begin(); it != children.end(); ++it) {
			if(c == (*it)->literal) return *it;
		}
		return NULL;
	}

};

/*
char getNextChar() {
	char c;
	getline(cin, s, delim);
	return s;
}
*/

Lz_pair get_next_pair(Dictionary_Node* root) {
	char c;
	if (((c = getchar()) == EOF) || match_length == MAX_MATCH_LENGTH) {
		end_of_file = true;
		Lz_pair retval = {root->index, 0x00};
		return retval;
	}
	if (verbose) printf("%c", c);
	Dictionary_Node* child;
	if ((child = root->getChild(c)) == NULL) {
		child = new Dictionary_Node(node_count++, c);
		root->addChild(child);
		Lz_pair retval = {root->index, c};
		return retval;
	} else {
		match_length++;
		return get_next_pair(child);
	}
}

void compress() {
	vector<Lz_pair> data;
	Dictionary_Node* root = new Dictionary_Node(node_count++, 0x00);
	Lz_pair current_pair;
	while (!end_of_file) {
		match_length = 0;
		current_pair = get_next_pair(root);
		data.push_back(current_pair);
		if (verbose) printf("---(%i,%c)\n", current_pair.prefix_index, current_pair.literal);
	}

	//Print data bytewise; 3 bytes for index, 1 byte for length, then character stream
	for (vector<Lz_pair>::iterator it = data.begin(); it != data.end(); ++it) {
		unsigned int index = it->prefix_index;
		//char length = (char) it->literal;
		putchar((char) ((index & 0x00ff0000) >> 16));
		putchar((char) ((index & 0x0000ff00) >> 8));
		putchar((char) (index & 0x000000ff));
		if (verbose) printf("(%i, %i, %i) %c\n", (index & 0x00ff0000), (index & 0x0000ff00), (index & 0x000000ff), it->literal);
		else printf("%c", it->literal);
	}
}

int main(int argc, char **argv) {

	if (argc > 1 && strcmp("-v", argv[1]) == 0) {
		verbose = true;
	}
	compress();
	return 0;
}