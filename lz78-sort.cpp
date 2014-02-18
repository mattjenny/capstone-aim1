#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <deque>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <list>

using namespace std;

struct Dictionary_Pair {
	unsigned int prefix_index;
	char literal;
};

deque<Dictionary_Pair> dictionary;
vector<char> data;
bool verbose = false;
unordered_set<char> unique_lit;
list<char> lit;
unordered_map<char, unsigned int> indices;
unsigned int pair_count = 0;

bool compare_ignore_case(char first, char second) {
	if (tolower(first) <= tolower(second)) return true;
	else return false;
}

void process_index(int index) {
	if (index == 0) return;
	else if (index > dictionary.size()) {
		printf("Index out of bounds.");
		return;
	}
	else {
		Dictionary_Pair pair = dictionary.at(index);
		process_index(pair.prefix_index);
		data.push_back(pair.literal);
	}
}

void process_pair(int index, char literal) {
	pair_count++;
	if (index > dictionary.size()) {
		printf("Index out of bounds.");
		return;
	}
	Dictionary_Pair current_pair = {index, literal};
	unique_lit.insert(literal);
	dictionary.push_back(current_pair);
}

void preprocess() {

	char c, literal;
	unsigned int i1, i2, i3, index;
	while ((c = getchar()) != EOF) {
		i1 = (unsigned int) c;
		i2 = (unsigned int) getchar();
		i3 = (unsigned int) getchar();
		index = ((i1 << 16) & 0x00ff0000) + ((i2 << 8) & 0x0000ff00) + i3;
		literal = getchar();
		if (verbose) printf("(i1 = %u, i2 = %u, i3 = %u) -> %u\n", i1, i2, i3, index);
		process_pair(index, literal);
		if (verbose) printf ("---(%i,%c)\n", index, literal);
	}

	/*
	while (prev < text.length()) {
		string literal;
		int index = (((int) text.at(prev)) << 16) + (((int) text.at(prev+1)) << 8) + ((int) text.at(prev+2)); //Re-create int.  Endianness makes this tough to do cleanly.
		char length = text.at(prev+3);
		if (length > 0) literal = text.substr(prev+4, length);
		else literal = "";
		process_pair(index, literal);
		prev += (length + 4); //step forward to the start of the next pair
	}
	*/

	for (unordered_set<char>::iterator set_iterator = unique_lit.begin(); set_iterator != unique_lit.end(); ++set_iterator) {
		lit.push_back(*set_iterator);
	}

	lit.sort(compare_ignore_case);
	unsigned int count = 0;
	for (list<char>::iterator it = lit.begin(); it != lit.end(); ++it) {
		indices.insert(make_pair<char, unsigned int>(*it, count));
		count++;
	}
}

void sort() {
	
	int i, j;
	unsigned int counts_matrix[dictionary.size()][lit.size()];
	unsigned int sum_vector[lit.size()];

	for (i=0; i<pair_count; i++) {
		for (j=0; j<lit.size(); j++) {
			counts_matrix[i][j] = 0;
		}
	}

	for (i=0; i<lit.size(); i++) {
		sum_vector[i] = 0;
	}

	for (i=0; i<dictionary.size(); i++) {
		Dictionary_Pair pair = dictionary.at(i);
		if (pair.prefix_index == 0) {
			unsigned int index = indices.at(pair.literal);
			counts_matrix[i][index] = 1;
			sum_vector[index]++;
		} else {
			for (j=0; j<lit.size(); j++) {
				counts_matrix[i][j] = counts_matrix[pair.prefix_index-1][j];
				sum_vector[j] += counts_matrix[pair.prefix_index-1][j];
			}
			unsigned int index = indices.at(pair.literal);
			counts_matrix[i][index] += 1;
			sum_vector[index]++;
		}
	}

	//Print the results
	printf("RESULTS:\n");
	i = 0;
	for (list<char>::iterator it = lit.begin(); it != lit.end(); ++it) {
		printf("(%c,%u)\n", *it, sum_vector[i]);
		i++;
	}
}

int main(int argc, char** argv) {
	if (argc > 1 && strcmp("-v", argv[1]) == 0) {
		verbose = true;
	}

	preprocess();
	sort();
	return 0;
}