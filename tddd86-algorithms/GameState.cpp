/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 *
 * @axega544 @margu424
 * Implementation of gamestate class
 */

#include "GameState.h"
#include "utilities.h"
#include "constants.h"
#include <iostream>

GameState::GameState(){}

/*
 * constuctor
 * crate a game state with number of robot specifed
 */
GameState::GameState(int numberOfRobots) {
    for (int i = 0; i < numberOfRobots; i++) {
        Robot *robot = new Robot();
        while (!isEmpty(*robot)) {
            delete robot;
            robot = new Robot();
        }

        robotPtrs.push_back(robot);
    }
    teleportHero();

}

GameState::~GameState(){
    for (int i = 0; i < robotPtrs.size(); i++){
        delete robotPtrs[i];
    }
}

/*
 * copy given game state to this game state
 */
GameState& GameState::operator =(const GameState &gameState){
    if (this != &gameState){
        GameState copy(gameState); // GameState(gameState);
        std::swap(robotPtrs, copy.robotPtrs);
        std::swap(hero, copy.hero);
    }
    return *this;
}


/*
 * copy consrtuctor
 */
GameState::GameState(const GameState &gameState){
    for (Robot* robot : gameState.robotPtrs){
        this->robotPtrs.push_back(robot->duplicate());
    }
    hero = gameState.getHero();
}

/*
 * draw scene
 */
void GameState::draw(QGraphicsScene *scene) const {
    scene->clear();
    for (size_t i = 0; i < robotPtrs.size(); ++i)
        robotPtrs[i]->draw(scene);
    hero.draw(scene);
}

void GameState::teleportHero() {
    do hero.teleport();
    while (!isEmpty(hero));
}

void GameState::moveRobots() {
    for (unsigned int i = 0; i < robotPtrs.size(); i++)
        robotPtrs[i]->moveTowards (hero);
}

/*
 * counten handel collision
 * dead robot gets replaced by junk
 */
int GameState::countCollisions() {
    int numberDestroyed = 0;
    for (unsigned int i = 0; i < robotPtrs.size(); i++) {
        bool collision = (countRobotsAt (*robotPtrs[i]) > 1);
        if (collision && !robotPtrs[i]->isJunk()) {
            robotPtrs.push_back(new Junk(*robotPtrs[i]));
            delete robotPtrs[i];
            robotPtrs[i] = robotPtrs[robotPtrs.size()-1];
            robotPtrs.pop_back();
            numberDestroyed++;
        }
    }
    return numberDestroyed;
}

bool GameState::anyRobotsLeft() const {
    for (int i = 0; i < robotPtrs.size(); i++){
        if (!robotPtrs[i]->isJunk()) return true;
    }
    return false;
}

bool GameState::heroDead() const {
    return !isEmpty(hero);
}

bool GameState::isSafe(const Unit& unit) const {
    for (unsigned int i = 0; i < robotPtrs.size(); i++)
        if (robotPtrs[i]->attacks(unit)) return false;
    return true;
}

void GameState::moveHeroTowards(const Unit& dir) {
    hero.moveTowards(dir);
}

Hero GameState::getHero() const {return hero;}

/*
 * Free of robots and junk only
 */
bool GameState::isEmpty(const Unit& unit) const {
    return (countRobotsAt(unit) == 0);
}

/*
 * How many robots are there at unit?
 */
int GameState::countRobotsAt(const Unit& unit) const {
    int count = 0;
    for (size_t i = 0; i < robotPtrs.size(); ++i) {
        if (robotPtrs[i]->at(unit))
            count++;
    }
    return count;
}
