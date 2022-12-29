#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
using namespace std;

const int WORD_LENGTH = 5;

bool isValid(string s);
void toUpper(string &s);
string randomWord();
void print();

int main () {
    int numTries = 6;
    vector<string> tries(numTries);
    vector<vector<int> > matches(numTries, vector<int>(WORD_LENGTH));

    string input;
    int currentTry = 0;

    while (currentTry < numTries) {
        do {
            cout << "Enter a guess (Q to quit): ";
            cin >> input;
            toUpper(input);
        } while (input != "Q");

        if (input == "Q") {
            cout << "Thank you for playing!" << endl;
            break;
        }
        currentTry++;
    }
    
}

void toUpper(string &s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
}