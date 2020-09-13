TEMPLATE = app
QT += widgets
HEADERS += mainwindow.h \
    utilities.h \
    constants.h \
    Unit.h \
    Junk.h \
    Hero.h \
    Robot.h \
    GameState.h \
    qgameoverwindow.h \
    qresetbutton.h
SOURCES += mainwindow.cpp \
    robotsmain.cpp \
    utilities.cpp \
    Unit.cpp \
    Junk.cpp \
    Hero.cpp \
    Robot.cpp \
    GameState.cpp \
    qgameoverwindow.cpp \
    qresetbutton.cpp
QMAKE_CXXFLAGS += -std=c++11
macx {
    cache()
}
