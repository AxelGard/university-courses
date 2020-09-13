/*
 * TDDD86 Boggle
 * This file contains the main program and user interface for running your
 * Boggle game.  We provide you a skeleton of this file that contains a shell
 * of the overall logic, but you must complete the playOneGame function.
 *
 * The playOneGame function talks to the Boggle class that you will write.
 * This file should contain all user interaction (cout / cin), while the Boggle
 * class should contain ALL game state such as the 16 letter cubes, the set of
 * words that have been formed, the algorithms for searching for words, etc.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 */

#include <fstream>
#include <iostream>
#include <string>
#include "random.h"
#include "strlib.h"
#include "Boggle.h"
#include "bogglemain.h"

using namespace std;

int main() {
    intro();

    // play games repeatedly until user decides to quit
    Boggle boggle;
    while (true) {
        playOneGame(boggle);
        cout << endl;
        if (!yesOrNo("Play again (Y/N)? ")) {
            break;
        }
    }

    cout << "Have a nice day." << endl;
    return 0;
}

/*
 * Explains the program to the user.
 */
void intro() {
    cout << "Welcome to TDDD86 Boggle!" << endl;
    cout << "This game is a search for words on a 2-D board of letter cubes." << endl;
    cout << "The good news is that you might improve your vocabulary a bit." << endl;
    cout << "The bad news is that you're probably going to lose miserably to" << endl;
    cout << "this little dictionary-toting hunk of silicon." << endl;
    cout << "If only YOU had a gig of RAM!" << endl;
    cout << endl;
    cout << "Press Enter to begin the game ... ";
    string line;
    getline(cin, line);
}

/*
 * Prompts the user to answer a yes/no question and returns true if the user
 * typed 'yes' (or anything that starts with a 'y', case-insensitively),
 * false if the user types anything that starts with 'n', or re-prompts if
 * the user doesn't type a 'y' or 'n' word.
 */
bool yesOrNo(string prompt) {
    cout << prompt;
    while (true) {
        string answer;
        getline(cin, answer);
        answer = trim(toLowerCase(answer));
        if (startsWith(answer, 'y')) {
            return true;
        } else if (startsWith(answer, 'n')) {
            return false;
        } else {
            cout << "Please type a word that begins with 'y' or 'n'." << endl;
        }
    }
}
