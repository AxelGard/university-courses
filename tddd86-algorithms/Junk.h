/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef JUNK_H
#define JUNK_H

#include "Robot.h"
#include <QGraphicsScene>

class Junk : public Robot {
public:
    Junk();
    Junk(const Robot &robot);

    /*
    * Draws this junk onto the given QGraphicsScene.
    */
    void draw(QGraphicsScene* scene) const;
    void moveTowards(const Unit &u);
    bool attacks(const Unit& u) const override;
    bool isJunk();
    virtual Robot* duplicate() const;
};



#endif // JUNK_H
