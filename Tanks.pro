TEMPLATE = app
TARGET = Tanks
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/engine/engine.cpp \
    src/engine/renderer.cpp \
    src/game_state/game.cpp \
    src/appconfig.cpp \
    src/objects/object.cpp \
    src/objects/tank.cpp \
    src/engine/spriteconfig.cpp \
    src/objects/enemy.cpp \
    src/objects/player.cpp \
    src/objects/bullet.cpp \
    src/objects/brick.cpp \
    src/objects/eagle.cpp \
    src/game_state/menu.cpp \
    src/game_state/scores.cpp \
    src/objects/bonus.cpp \
    src/game_state/server.cpp \
    src/engine/servertcp.cpp \
    src/engine/event.cpp \
    src/game.cpp \
    app.cpp

CC = C:\MinGW\bin\mingw32-g++.exe
CFLAGS = -c -Wall -std=c++11
LFLAGS = -mwindows -O

#SDL lib
LIBS += -L$$PWD/SDL/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
#Winsock lib
LIBS += -lws2_32

DEPENDPATH += $$PWD/SDL/i686-w64-mingw32/include
INCLUDEPATH += $$PWD/SDL/i686-w64-mingw32/include

HEADERS += \
    src/engine/engine.h \
    src/engine/renderer.h \
    src/game_state/game.h \
    src/appconfig.h \
    src/objects/object.h \
    src/objects/tank.h \
    src/engine/spriteconfig.h \
    src/type.h \
    src/objects/enemy.h \
    src/objects/player.h \
    src/objects/bullet.h \
    src/objects/brick.h \
    src/objects/eagle.h \
    src/game_state/menu.h \
    src/game_state/scores.h \
    src/objects/bonus.h \
    src/game_state/server.h \
    src/engine/servertcp.h \
    src/engine/event.h \
    src/game.h \
    src/game_state/gamestate.h \
    app.h
