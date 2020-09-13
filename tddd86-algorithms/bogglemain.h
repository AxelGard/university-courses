/*
 * TDDD86 Boggle
 * This file declares required function prototypes that are defined in
 * our provided bogglemain.cpp and your boggleplay.cpp that you will write.
 * See the respective .cpp files for implementation comments for each function.
 * Please do not modify this provided file.
 */

#ifndef _bogglemain_h
#define _bogglemain_h

#include "Boggle.h"
#include <string>
using namespace std;

void intro();
void playOneGame(Boggle& boggle);
bool yesOrNo(string prompt);
void clearConsole();

#endif
