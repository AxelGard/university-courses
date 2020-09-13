/**
 * Boggle header file containing method definitions etc.
 *
 * Axega544 & Margu424
 */

#ifndef _boggle_h
#define _boggle_h

#include "grid.h"
#include "lexicon.h"
#include <set>
#include <iostream>
#include <string>

using namespace std;
struct Point{
    Point(){}
    int x=0;
    int y=0;
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;

    void shuffleBoard(bool useDefaultBoard);
    void printBoard();
    bool guess(const string& word, string& errorMsg);
    void findWords();
    vector<Point> findNeighbors(Point& point, vector<Point> visited);
    void findWordsRecursivly(const string& chosen, set<string>& found, vector<Point>& visited);

    // public getters
    set<string>& getPlayerWords();
    int getPlayerScore();
    set<string>& getWords();
private:
    Grid<char> board = Grid<char>(BOARD_SIZE, BOARD_SIZE);
    Lexicon lex = Lexicon(DICTIONARY_FILE);
    set<string> words = set<string>();
    set<string> playerWords = set<string>();
    int playerScore = 0;
};

#endif
