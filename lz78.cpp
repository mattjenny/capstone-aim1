#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

unsigned int node_count = 0;
int match_length = 0;
static int MAX_MATCH_LENGTH = 255;
char delim = 0x20;
bool verbose = false;

typedef struct {
	unsigned int prefix_index;
	string literal;
} Lz_pair;

struct Dictionary_Node {

	unsigned int index;
	string literal;
	vector<Dictionary_Node*> children;

	Dictionary_Node(unsigned int index_in, string literal_in) {
		index = index_in;
		literal = literal_in;
	}

	void addChild(Dictionary_Node* child) {
		children.push_back(child);
	}

	Dictionary_Node* getChild(string s) {
		for(vector<Dictionary_Node*>::iterator it = children.begin(); it != children.end(); ++it) {
			if(s.compare((*it)->literal) == 0) return *it;
		}
		return NULL;
	}

};

string getNextString() {
	string s;
	getline(cin, s, delim);
	return s;
}

Lz_pair get_next_pair(Dictionary_Node* root) {
	if (cin.eof() || match_length == MAX_MATCH_LENGTH) {
		Lz_pair retval = {root->index, ""};
		return retval;
	}
	Dictionary_Node* child;
	string s = getNextString();
	if ((child = root->getChild(s)) == NULL) {
		child = new Dictionary_Node(node_count++, s);
		root->addChild(child);
		Lz_pair retval = {root->index, s};
		return retval;
	} else {
		match_length++;
		return get_next_pair(child);
	}
}

void compress() {
	vector<Lz_pair> data;
	Dictionary_Node* root = new Dictionary_Node(node_count++, "");
	Lz_pair current_pair;
	while (!cin.eof()) {
		match_length = 0;
		current_pair = get_next_pair(root);
		data.push_back(current_pair);
		if (verbose) printf("(%i,%s)\n", current_pair.prefix_index, current_pair.literal.c_str());
	}

	//Print data bytewise; 3 bytes for index, 1 byte for length, then character stream
	for (vector<Lz_pair>::iterator it = data.begin(); it != data.end(); ++it) {
		int index = it->prefix_index;
		char length = (char) it->literal.length();
		putchar(index & 0x00ff0000);
		putchar(index & 0x0000ff00);
		putchar(index & 0x000000ff);
		putchar(length);
		printf("%s", it->literal.c_str());
	}
}

int main(int argc, char **argv) {

	if (argc > 1 && strcmp("-v", argv[1]) == 0) {
		verbose = true;
	}
	compress();
	return 0;
}