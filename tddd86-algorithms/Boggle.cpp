/*
 * Implementation of Boggle class.
 *
 * Axega544 & Margu424
 */

#include <sstream>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"
#include "foreach.h"
#include "set.h"
#include <queue>

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = {        // the letters on all 6 sides of every cube
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};



/**
 * Shuffles board or give you the default board if useDefaultBoard is true
 */
void Boggle::shuffleBoard(bool useDefaultBoard){
    words = set<string>();
    playerWords = set<string>();
    if (useDefaultBoard){
        char chars[] = {'F', 'Y', 'C', 'L', 'I', 'O', 'M', 'G', 'O', 'R', 'I', 'L', 'H', 'J', 'H', 'U'};
        for (int i = 0; i < NUM_CUBES; i++){
            char c = chars[i];
            board[i/BOARD_SIZE][i%BOARD_SIZE] = c;
        }
    } else {
        shuffle(CUBES, NUM_CUBES);
        for (int i = 0; i < NUM_CUBES; i++){
            char c = CUBES[i][randomInteger(0, CUBE_SIDES-1)];
            board[i/BOARD_SIZE][i%BOARD_SIZE] = c;
        }
    }
}

/**
 * Checks if guess is correct. Return true and increases player score if correct
 * othewise it returns false
 */
bool Boggle::guess(const string& word, string& message){
    if (word.size() < 4) {
        message = "to few characters ";
        return false;
    }
    if (!lex.contains(word)){
        message = "not in lexicon";
        return false;}
    if (playerWords.count(word)){
        message = "Already found ";
        return false;}
    if (words.count(word)){
        playerWords.insert(word);
        words.erase(word);
        playerScore += word.size() - 3;
        message = "You found a new word! \"" + word + "\"";
        return true;
    }
    message = "word not formable ";
    return false;
}

/**
 * Return a vector of Points that are the neighbours of point not already visited
 */
vector<Point> Boggle::findNeighbors(Point& point, vector<Point> visited){
    vector<Point> neighbors = vector<Point>();
    for (int i = -1; i < 2; i++){
        for (int j = -1; j< 2; j++){
            int x = point.x+i;
            int y = point.y+j;
            if (0 <= x && x < BOARD_SIZE && 0 <= y && y < BOARD_SIZE){
                bool visitedBefore = false;
                for (int k = 0; k < visited.size(); k++){
                    if (visited[k].x == x && visited[k].y == y){
                        visitedBefore = true;
                        break;
                    }
                }
                if (!visitedBefore){
                    neighbors.push_back(Point(x, y));

                }
              }
        }
    }
    return neighbors;
}

/**
 * Uses recursion to find words of each neighbour
 */
void Boggle::findWordsRecursivly(const string& chosen, set<string>& found, vector<Point>& visited){
    if (!lex.containsPrefix(chosen)) return;
    if (chosen.size() >= 4 && lex.contains(chosen) && !found.count(chosen)){
        //std::cout<< "word : " << chosen << std::endl;
        found.insert(chosen);
    }
    vector<Point> neighbors = findNeighbors(visited[visited.size()-1], visited);
    for (int i = 0; i < neighbors.size(); i++){
       char letter = board.get(neighbors.at(i).x, neighbors.at(i).y);
       visited.push_back(neighbors.at(i));
       string newChosen = chosen + letter;
       findWordsRecursivly(newChosen, found, visited);
       visited.pop_back();
    }
}

/**
 * Calls findWordsRecursively for each starting letter in board
 */
void Boggle::findWords(){
    for (int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++){
        int x = i / BOARD_SIZE;
        int y = i % BOARD_SIZE;
        string word = "";
        word.push_back(board.get(x,y));
        vector<Point> visited = vector<Point>();
        visited.push_back(Point(x,y));
        findWordsRecursivly(word, words, visited);
    }

}

/**
 * prints board
 */
void Boggle::printBoard(){
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            std::cout<< board[i][j];
        }
        std::cout<<std::endl;
    }
}


int Boggle::getPlayerScore(){
    return playerScore;
}

set<string>& Boggle::getPlayerWords(){
    return playerWords;
}

set<string>& Boggle::getWords(){
    return words;
}

