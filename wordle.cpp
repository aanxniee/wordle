#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <curl/curl.h>
#include "config.h"

using namespace std;

const int WORD_LENGTH = 5;
const int MAX_GUESSES = 6;
const int NOT_MATCHED = 0;
const int PARTIAL_MATCHED = 1;
const int MATCHED = 2;


// ----------------------------------------------------------------

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ----------------------------------------------------------------

bool isValid(string s);
void toUpper(string &s);
string randomWord();
void print(vector<string>tries, vector<vector<int> > macthes, int currentTry);

// ----------------------------------------------------------------

int main () {
    vector<string> guesses(MAX_GUESSES);
    vector<vector<int> > matches(MAX_GUESSES, vector<int>(WORD_LENGTH));

    string input;
    int currentGuess = 0;
    string word = randomWord();
    toUpper(word);

    cout << word << endl;
    print(guesses, matches, currentGuess);

    while (currentGuess < MAX_GUESSES) {
        do {
            cout << "ENTER A GUESS (Q TO QUIT): ";
            cin >> input;
            toUpper(input);
        } while (input != "Q" && !isValid(word));

        if (input == "Q") {
            cout << endl;
            cout << "THANK YOU FOR PLAYING!" << endl;
            break;
        }

        guesses[currentGuess] = input;

        print(guesses, matches, currentGuess);
        currentGuess++;
    }
    
}

// ----------------------------------------------------------------

bool isValid(string s) {
    return (s.length() != 5 && s.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") ==
        std::string::npos);
}

void toUpper(string &s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
}

string randomWord() {
    string word;
    CURL *hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_URL, API::URL);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, API::KEY);
    headers = curl_slist_append(headers, API::HOST);
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &word);

    CURLcode ret = curl_easy_perform(hnd);
    curl_easy_cleanup(hnd);

    return word;
}

void print(vector<string>guesses, vector<vector<int> > matches, int currentGuess) {
    system("clear");
    cout << " ---------------" << endl;
    cout << "|  W O R D L E  |" << endl ;
    cout << " ---------------" << endl << endl;
}