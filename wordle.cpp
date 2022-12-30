#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <curl/curl.h>
#include "config.h"
using namespace std;

const int WORD_LENGTH = 5;

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
void print();

int main () {
    int numTries = 6;
    vector<string> tries(numTries);
    vector<vector<int> > matches(numTries, vector<int>(WORD_LENGTH));

    string input;
    int currentTry = 0;
    string word = randomWord();

    cout << word << endl;

    while (currentTry < numTries) {
        do {
            cout << "Enter a guess (Q to quit): ";
            cin >> input;
            toUpper(input);
        } while (input != "Q" && !isValid(word));

        if (input == "Q") {
            cout << "Thank you for playing!" << endl;
            break;
        }

        currentTry++;
    }
    
}

bool isValid(string s) {
    return (s.length() != 5 && s.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") ==
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