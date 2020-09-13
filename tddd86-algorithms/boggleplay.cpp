/*
 * Contains game logic for playing one game of boggle
 *
 * Axega544 & Margu424
 */
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"

/**
 * prints player score and words found
 */
void printScore(Boggle& boggle){
    set<string> playerWords = boggle.getPlayerWords();
    string msg = "Your words (" + std::to_string(playerWords.size()) + "): {";
    for (string word : playerWords){
        msg += "\"" + word + "\",";
    }
    msg = msg.substr(0,msg.size()-1);
    msg += "}\nYour score: " + std::to_string(boggle.getPlayerScore());
    std::cout << msg << std::endl;
}

/**
 * prints words left to find and their combined score.
 * also tells the user if he/she has won.
 */
void computersTurn(Boggle& boggle){
    int score = 0;
    int playerScore = boggle.getPlayerScore();
    set<string> words = boggle.getWords();
    string str = "It\'s my turn! \nMy words (" + std::to_string(words.size()) + "): {";
    for (const string& word : words){
        str += "\"" + word + "\",";
        score += word.size() - 3;
    }
    str = str.substr(0, str.size()-1);
    str += "}\nMy score: " + std::to_string(score) + "\n";
    if (score > playerScore){
        str += "Ha ha ha, I destroyed you. Better luck next time, puny human!";
    } else if (score == playerScore) {
        str += "Draw, I was lagging";
    } else {
        str += "I lose, impossible! You cheating piece of s***!";
    }
    std::cout << str << std::endl;
}


/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    clearConsole();

    std::cout << "Do you want to generate a random board? ";
    string ans = "";
    std::getline(std::cin, ans);
    boggle.shuffleBoard(ans[0] == 'y' ? false : true);

    std::cout << "It\'s your turn!" << std::endl;
    boggle.printBoard();
    boggle.findWords();
    string guess;
    do {
        std::cout << "Type a word (or press Enter to end your turn): ";
        std::getline(std::cin, guess);
        std::transform(guess.begin(), guess.end(), guess.begin(), ::toupper); // covert guess to upper case
        string msg;
        bool correct = boggle.guess(guess, msg);
        std::cout << msg << std::endl;
        if (correct) printScore(boggle);
    } while (guess.size() != 0);
    std::cout << std::endl;
    computersTurn(boggle);
}



/*
 * Erases all currently visible text from the output console.
 */
void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("clear");
#endif
}
