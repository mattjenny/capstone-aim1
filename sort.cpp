#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

char delim = 0x20;

int main(){
    // Empty vector holding all names from file
    vector<string> words;

    // Read words
    string word;
    while(getline(cin, word, delim)){
      words.push_back(word);
    }

    sort(words.begin(), words.end());

    // Loop to print names
    for (size_t i = 0; i < words.size(); i++)
        cout << words[i] << '\n';
}
