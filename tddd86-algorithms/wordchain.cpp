/*
 * Martin Gustafsson @margu424 && Axel Gard @axega544
 *
 * takes two strings w1 w2 and finds a chain of words between
 * those words by changing one letter of the word each
 * step of the way, from w1 to w2.
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
#include <set>
using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

/*
* given a string word and a map dictionar,
* will return a vector of closest neighbours
*/
vector<string> getNeighbours(const string& word, const set<string>& dictionary){
    vector<string> neighbours;
    for (unsigned i = 0; i < word.length(); i++){
        for (unsigned j = 0; j < ALPHABET.length(); j ++){
            string newWord = word;
            newWord[i] = ALPHABET[j];
            if (newWord != word && dictionary.count(newWord)){
                neighbours.push_back(newWord);
            }
        }
    }
    return neighbours;
}

/*
* Given two strings w1 and w2,
* prints word chain of the two words.
*/
void wordChain(const string& w1, const string& w2){

    const string filename = "dictionary.txt";
    ifstream dictionaryFile(filename);
    if (!dictionaryFile.is_open()){
        cout << "Error, needed dictonary could not be found! " << endl;
    }

    set<string> dictionary;
    string word;
    set<string> usedWords;
    while(!dictionaryFile.eof()){
        getline(dictionaryFile, word);
        dictionary.insert(word);
    }

    queue<stack<string>> stackQueue;
    stack<string> initStack;
    initStack.push(w1);
    stackQueue.push(initStack);

    usedWords.insert(w1);
    while (!stackQueue.empty()) {
        stack<string> partialChain = stackQueue.front();
        stackQueue.pop();
        if (partialChain.top() == w2){
            while (!partialChain.empty()) {
                cout << partialChain.top() << " ";
                partialChain.pop();
            }
            cout << endl;
            cout << "have a nice day :)" << endl;
            return;
        }
        else{
            vector<string> neighbours = getNeighbours(partialChain.top(), dictionary);
            for (unsigned i=0; i < neighbours.size(); i++){
                string word = neighbours.at(i);
                if (!usedWords.count(word)){
                    stack<string> tempStack {partialChain};
                    tempStack.push(word);
                    stackQueue.push(tempStack);
                    usedWords.insert(word);

                }
            }
        }
    }
}


int main() {
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    string w1;
    string w2;
    cout << "Please type two words: ";
    cin >> w1;
    cin >> w2;
    wordChain(w1, w2);


    return 0;
}
