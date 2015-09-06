TEMPLATE = app
TARGET = Tanks
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/app.cpp \
    src/appconfig.cpp \
    src/main.cpp \
    src/game/game.cpp \
    src/game/engine/engine.cpp \
    src/game/engine/renderer.cpp \
    src/game/engine/spriteconfig.cpp \
    src/game/game_state/battle.cpp \
    src/game/game_state/menu.cpp \
    src/game/game_state/scores.cpp \
    src/game/game_state/server.cpp \
    src/game/objects/bonus.cpp \
    src/game/objects/brick.cpp \
    src/game/objects/bullet.cpp \
    src/game/objects/eagle.cpp \
    src/game/objects/enemy.cpp \
    src/game/objects/object.cpp \
    src/game/objects/player.cpp \
    src/game/objects/tank.cpp \
    src/network/servertcp.cpp \
    src/event/event.cpp \
    src/scopelock.cpp \
    src/event/eventstore.cpp \
    src/shareddata.cpp \
    src/network/network.cpp \
    src/appthread.cpp \
    src/network/clienttcp.cpp \
    src/network/tcpconnection.cpp \
    src/game/game_state/client.cpp \
    src/game/handler/handler.cpp \
    src/game/game_state/networkbattle.cpp \
    src/myqueue.cpp

CC = C:\MinGW\bin\mingw32-g++.exe
CFLAGS = -c -Wall -std=c++11
#LFLAGS = -mwindows -O

#SDL lib
LIBS += -L$$PWD/SDL/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
#Winsock lib
LIBS += -lws2_32

DEPENDPATH += $$PWD/SDL/i686-w64-mingw32/include
INCLUDEPATH += $$PWD/SDL/i686-w64-mingw32/include

HEADERS += src/app.h \
    src/appconfig.h \
    src/appthread.h \
    src/type.h \
    src/game/game.h \
    src/game/engine/engine.h \
    src/game/engine/renderer.h \
    src/game/engine/spriteconfig.h \
    src/game/game_state/battle.h \
    src/game/game_state/gamestate.h \
    src/game/game_state/menu.h \
    src/game/game_state/scores.h \
    src/game/game_state/server.h \
    src/game/objects/bonus.h \
    src/game/objects/brick.h \
    src/game/objects/bullet.h \
    src/game/objects/eagle.h \
    src/game/objects/enemy.h \
    src/game/objects/object.h \
    src/game/objects/player.h \
    src/game/objects/tank.h \
    src/network/servertcp.h \
    src/event/event.h \
    src/scopelock.h \
    src/event/eventstore.h \
    src/shareddata.h \
    src/network/network.h \
    src/network/tcpconnection.h \
    src/network/clienttcp.h \
    src/game/game_state/client.h \
    src/game/handler/handler.h \
    src/game/game_state/networkbattle.h \
    src/myqueue.h
