#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <curl/curl.h>
#include "config.h"

using namespace std;

// declare constant global variables
const int WORD_LENGTH = 5;
const int MAX_GUESSES = 6;
const int NOT_MATCHED = 0;
const int PARTIAL_MATCHED = 1;
const int MATCHED = 2;


// ----------------------------------------------------------------

// converts type CODEcurl into a string
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ----------------------------------------------------------------

// function declarations
bool isValid(string s); // checks if a string is valid
void toUpper(string &s); // converts a string to uppercase
string randomWord(); // generates a random 5 letter english word using Random Number API
void checkMatch(string input, string word, vector<vector<int> > &matches, int currentGuess); // checks for matching letters between the generated word and user's input
bool allMatched(string input, string word); // checks if the user entered the correct guess
void print(vector<string>tries, vector<vector<int> > matches, int currentTry); // displays the user's guess

// ----------------------------------------------------------------

int main () {
    vector<string> guesses(MAX_GUESSES); // stores the user's guesses
    // stores the correctness of each of the user's guess via 0, 1, and 2's
    // ie. word = MAIZE, guess = MEATY, the number stored will be 2 1 1 0 0 
    vector<vector<int> > matches(MAX_GUESSES, vector<int>(WORD_LENGTH)); 

    string input;
    bool win = false;
    int currentGuess = 0;
    
    string word = randomWord(); // generate a random word
    toUpper(word); 
    cout << endl;

    // main game loop, allows user to guess at max 6 times
    while (currentGuess < MAX_GUESSES) {
        
        // do-while loop, continually take input from user as long as their input is valid and they do not choose to quit
        do {
            cout << "ENTER YOUR GUESS (Q TO QUIT): ";
            cin >> input;
            toUpper(input);
        } while (input != "Q" && !isValid(input));
        
        // break out of the main game loop if they choose to quit
        if (input == "Q") {
            cout << "QUIT GAME" << endl;
            break;
        }

        // store their guess in the vector
        guesses[currentGuess] = input;

        // for each guess, check the match and display it to the console
        checkMatch(input, word, matches, currentGuess);
        print(guesses, matches, currentGuess);

        // if the user guessed correctly, end the game
        if (allMatched(input, word)) {
            cout << endl;
            cout << "YOU WIN! THE WORD WAS " << word << endl;
            win = true;
            break;
        }

        currentGuess++; // increment the number of guesses
    }

    if (!win) {
        cout << endl;
        cout << "GAME OVER! THE WORD WAS " << word << endl;
    }
}

// ----------------------------------------------------------------

// checks if a string is valid
bool isValid(string s) {
    // ensure string is 5 letters
    return s.length() == WORD_LENGTH && s.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos;;
}

// converts a string to uppercase
void toUpper(string &s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
}

// generates a random 5 letter english word using Random Number API
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

// checks for matching letters between the generated word and user's input
void checkMatch(string input, string word, vector<vector<int> > &matches, int currentGuess) {

    // initially set each word as 0 0 0 0 0 (NOT MATCHED)
    for (int i = 0; i < WORD_LENGTH; i++) {
        matches[currentGuess][i] = NOT_MATCHED;
    }

    // iterate through each guess to check match with the generated word 
    for (int i = 0; i < WORD_LENGTH; i++) {
        for (int j = 0; j < WORD_LENGTH; j++) {
            if (input[i] == word[j]) {
                if (i == j) {
                    // if the letters are at the correct index, it is a match
                    // ie. word = HELLO, guess = JERKY (E is a match) 
                    matches[currentGuess][i] = MATCHED;
                    break;
                }
                else {
                    // if the letters match but the index does not, it is a partial match
                    // ie. word = PLANE, guess = SHEEP (P and E are partial matches)
                    matches[currentGuess][i] = PARTIAL_MATCHED;
                }
            }
        }
    }
}

// checks if the user entered the correct guess
bool allMatched(string input, string word) {
    return (input == word);
}

// displays the user's guess
void print(vector<string>guesses, vector<vector<int> > matches, int currentGuess) {
    system("clear"); // clear console
    cout << "-----------------" << endl;
    cout << "   W O R D L E  " << endl ;
    cout << "-----------------" << endl << endl;

    // display each of the user's guesses and their correctness
    for (int i = 0; i <= currentGuess && i < MAX_GUESSES; i++) {

        string padding = "--";
        string text = " ";

        for (int j = 0; j < WORD_LENGTH; j++) {
            padding += "---";
            text += " ";

            // parse each of the user's guesses into characters (that way we can change each letter's colour depending on no match, partial match or match)
            char letter = toupper(guesses[i][j]);

            if (matches[i][j] == PARTIAL_MATCHED) {
                text += "\033[33m"; // turns text yellow, indicating partial match
            }
            else if (matches[i][j] == MATCHED) {
                text += "\033[32m"; // turns text green, indicating match
            }

            text += letter;
            text += " ";

            if (matches[i][j] == PARTIAL_MATCHED || matches[i][j] == MATCHED) {
                text += "\033[0m"; // coonverts text back to white
            }

        }
        if (i == 0) {
            cout << padding << endl;
        }

        cout << text << endl;
        cout << padding << endl;
    }
}