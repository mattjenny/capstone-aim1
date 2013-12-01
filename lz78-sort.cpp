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
	string literal;
};

deque<Dictionary_Pair> dictionary;
vector<string> data;
bool verbose = false;
unordered_set<string> unique_lit;
list<string> lit;
unordered_map<string, unsigned int> indices;
string text = "";
unsigned int pair_count = 0;

bool compare_ignore_case(string first, string second) {
	unsigned int i=0;
	while (i<first.length() && i<second.length()) {
		if (tolower(first[i]) < tolower(second[i])) return true;
		if (tolower(first[i]) > tolower(second[i])) return false;
		i++;
	}
	return (first.length() < second.length());
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

void process_pair(int index, string literal) {
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
	int prev = 0;
	while (prev < text.length()) {
		string literal;
		int index = (((int) text.at(prev)) << 16) + (((int) text.at(prev+1)) << 8) + ((int) text.at(prev+2)); //Re-create int.  Endianness makes this tough to do cleanly.
		char length = text.at(prev+3);
		if (length > 0) literal = text.substr(prev+4, length);
		else literal = "";
		process_pair(index, literal);
		prev += (length + 4); //step forward to the start of the next pair
	}

	for (unordered_set<string>::iterator set_iterator = unique_lit.begin(); set_iterator != unique_lit.end(); ++set_iterator) {
		lit.push_back(*set_iterator);
	}

	lit.sort(compare_ignore_case);
	unsigned int c = 0;
	for (list<string>::iterator it = lit.begin(); it != lit.end(); ++it) {
		indices.insert(make_pair<string, unsigned int>(*it, c));
		c++;
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
	for (list<string>::iterator it = lit.begin(); it != lit.end(); ++it) {
		printf("(%s,%u) ", it->c_str(), sum_vector[i]);
		i++;
	}
}

int main(int argc, char** argv) {
	if (argc > 1 && strcmp("-v", argv[1]) == 0) {
		verbose = true;
	}
	// Read cin into a string
	istreambuf_iterator<char> eos;
	text = string(istreambuf_iterator<char> (cin), eos);

	preprocess();
	sort();
	return 0;
}