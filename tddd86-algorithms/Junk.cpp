/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 *
 * @axega544 @margu424
 * Implementation of junk class
 */

#include "Junk.h"
#include "constants.h"
#include <iostream>

Junk::Junk() : Robot() {}
Junk::Junk(const Robot &robot) : Robot(robot) {}

void Junk::draw(QGraphicsScene *scene) const {
    Point corner = asPoint();
    scene->addEllipse(QRectF(corner.x * UNIT_WIDTH, corner.y * UNIT_HEIGHT,
                             JUNK_RADIUS, JUNK_RADIUS), QPen(), QBrush(JUNK_COLOR));
}

bool Junk::isJunk(){
    return true;
}

void Junk::moveTowards(const Unit& u){

}

bool Junk::attacks(const Unit& u) const{
    return false;
}

Robot* Junk::duplicate() const{
    return new Junk(*this);
}
