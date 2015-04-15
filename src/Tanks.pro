TEMPLATE = app
TARGET = Tanks
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    app.cpp \
    engine/engine.cpp \
    engine/renderer.cpp \
    engine/sound.cpp \
    app_state/game.cpp \
    appconfig.cpp \
    objects/object.cpp \
    objects/tank.cpp \
    engine/spriteconfig.cpp \
    objects/objectfactory.cpp \
    objects/enemy.cpp \
    objects/player.cpp \
    objects/bullet.cpp \
    objects/brick.cpp

CC = C:\MinGW\bin\mingw32-g++.exe
CFLAGS = -c -Wall -std=c++11
LFLAGS = -mwindows -O

LIBS += -L$$PWD/../SDL/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

DEPENDPATH += $$PWD/../SDL/i686-w64-mingw32/include
INCLUDEPATH += $$PWD/../SDL/i686-w64-mingw32/include

HEADERS += \
    app.h \
    engine/engine.h \
    engine/renderer.h \
    engine/sound.h \
    app_state/appstate.h \
    app_state/game.h \
    appconfig.h \
    tank.h \
    objects/object.h \
    objects/tank.h \
    engine/spriteconfig.h \
    type.h \
    objects/objectfactory.h \
    objects/enemy.h \
    objects/player.h \
    objects/bullet.h \
    objects/brick.h

