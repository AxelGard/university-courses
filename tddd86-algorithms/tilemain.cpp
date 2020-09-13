/*
 * TDDD86 Tiles
 * This client program uses your TileList class and contains the 'main'
 * function to set up the overall program's graphical user interface.
 * Your code should work properly with an unmodified version of this file.
 */

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow view;
    view.show();

    return a.exec(); // start Qt event loop
}
