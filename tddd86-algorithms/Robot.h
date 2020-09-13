/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "Unit.h"
#include <QGraphicsScene>

class Robot : public Unit {
public:
    Robot();
    Robot(const Robot &robot);
    virtual void draw(QGraphicsScene* scene) const;
    virtual bool isJunk();
    virtual Robot* duplicate() const;

};

#endif // ROBOT_H
