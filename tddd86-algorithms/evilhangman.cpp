/*
 * Martin Gustafsson @margu424 && Axel Gard @axega544
 *
 * plays the game hangman but cheats by changing the word it thinks of in order
 * to win more often.
 *
 * Soruces :
 * labb PM - https://www.ida.liu.se/~TDDD86/info/misc/labb2.pdf
 * C++ doc - http://www.cplusplus.com/doc/tutorial/files/
 *
 */

#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <math.h>
using namespace std;
using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

/*
 * Takes a char guessedChar and a vector<string> words and modifies words
 * by referense in order to put the words from the largest word family into it.
 * Also returns a boolean based on if the guess was correct or not.
 */
bool getLargestWordFamily(const char& guessedChar, vector<string>& words){
    //split words into word families
    map<int, vector<string>> wordfamilies;
    for (unsigned i = 0; i < words.size(); i++){
        string word = words.at(i);
        int value = 0;
        for (unsigned j = 0; j < word.length(); j++){
            if (word.at(j) == guessedChar){
                value += pow(2, j);
            }
        }
        if (!wordfamilies.count(value)){
            vector<string> tempVector;
            tempVector.push_back(word);
            wordfamilies.insert(pair<int, vector<string>>(value, tempVector));
        }
        else{
            wordfamilies.at(value).push_back(word);
        }
    }

    //Find the largest word family
    int largestWordFamily = 0;
    int largestWordFamilyKey = 0;
    for (auto it = wordfamilies.begin(); it != wordfamilies.end(); it++){
        int size_ = (it->second).size();
        if (size_ > largestWordFamily){
            largestWordFamily = size_;
            largestWordFamilyKey = it->first;
        }
    }

    words = wordfamilies.at(largestWordFamilyKey);

    //was the guess not correct?
    return (largestWordFamilyKey != 0);
}

/*
 * Asks the user for a char input and returns it when
 * it in the alphabet and not used before.
 */
char getUserGuess(vector<char> &guessedLetters){
    string answerString;
    while (answerString.length() != 1) {
        cout << "Guess: ";
        cin >> answerString;
        bool isInAlphabet;
        for (int i = 0; i < ALPHABET.length(); i++){
            if (ALPHABET.at(i) == answerString[0]){
                isInAlphabet = true;
                break;
            }
        }
        if (!isInAlphabet) {
            answerString = "";
            continue;
        }
        for (int i = 0; i < guessedLetters.size(); i++){
            if (guessedLetters.at(i) == answerString[0]){
                answerString = "";
                break;
            }
        }
    }

    char guessesChar = answerString[0];
    guessedLetters.push_back(guessesChar);
    return guessesChar;
}


/*
* game logic for hangman,
* takes a bool dubug, vector of strings words
* and a int guesses number of allowd guesses
* then returns a boolean based on if player won or not
* (true = player win, false = player lost)
*/
bool hangman(const bool &debug, vector<string> &words, int &guesses){

    vector<char> guessedLetters;
    string knownWord;
    for (unsigned i = 0; i < words[0].length(); i++){
        knownWord += "-";
    }

    // round loop
    while (guesses != 0) {

        // base info
        cout << "__________________________" << endl;
        cout << "Number of guesses left: " << guesses << endl;
        cout << "Already guessed letters are: ";
        for (unsigned i = 0; i < guessedLetters.size(); i++){
            cout << guessedLetters.at(i);
        }
        cout << endl;
        cout << knownWord << endl;

        //player wins
        if (knownWord.find("-") == string::npos){
            return true;
        }

        char guessesChar = getUserGuess(guessedLetters);

        bool correctGuess = getLargestWordFamily(guessesChar, words);
        if (!correctGuess) guesses--;

        for (unsigned i = 0; i < words[0].length(); i++){
            if (words[0][i] == guessesChar){
                knownWord[i] = guessesChar;
            }
        }

        if (debug){
            cout << "Words left in dictionary: ";
            for (unsigned i = 0; i < words.size(); i++){
                cout << words.at(i) << " ";
            }
            cout << endl;
        }
    }
    // game over, player lost
    return false;
}

/*
* main
* loads needed file
* sets game into a loop
*/
int main() {
    cout << "Welcome to Hangman." << endl;

    // Read file
    const string filename = "dictionary.txt";
    ifstream dictionaryFile(filename);
    if (!dictionaryFile.is_open()){
        cout << "Error, needed dictonary could not be found! " << endl;
        return 1;
    }

    // Map words from given file based on their length
    map<int, vector<string>> dictionary;
    string word;
    while(!dictionaryFile.eof()){
        getline(dictionaryFile, word);
        if (word.length() == 0){
            continue;
        }
        if (!dictionary.count(word.length())){
            vector<string> tempVector;
            tempVector.push_back(word);
            dictionary.insert(pair<int, vector<string>>(word.length(), tempVector));
        }
        else{
            dictionary.at(word.length()).push_back(word);
        }
    }

    // Game
    bool playGame = true;
    while (playGame) {
        int wordLength = 0;
        int guesses = 0;
        while (wordLength == 0) {
            cout << "Give length of word: ";
            cin >> wordLength;
            if (!dictionary.count(wordLength)){
                wordLength = 0;
            }
        }
        while (guesses <= 0) {
            cout << "Number of allowed guesses: ";
            cin >> guesses;
            if (guesses > 26){
                guesses = 26;
            }
        }
        bool debug = false;
        char answer;
        cout << "do you whant debug mode, y/n: ";
        cin >> answer;
        if (answer == 'y'){
            debug = true;
        }

        bool game = hangman(debug, dictionary.at(wordLength), guesses);

        if (game){
            cout << "YOU WIN!!" << endl;
        }
        else{
            cout << "you lost" << endl;
        }
        cout << "play again? y/n: ";
        cin >> answer;
        if (answer == 'n') playGame = false;

    }
    // end of main
    return 0;
}
