TEMPLATE = app
TARGET = Tanks
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/app.cpp \
    src/engine/engine.cpp \
    src/engine/renderer.cpp \
    src/engine/sound.cpp \
    src/app_state/game.cpp \
    src/appconfig.cpp \
    src/objects/object.cpp \
    src/objects/tank.cpp \
    src/engine/spriteconfig.cpp \
    src/objects/objectfactory.cpp \
    src/objects/enemy.cpp \
    src/objects/player.cpp \
    src/objects/bullet.cpp \
    src/objects/brick.cpp \
    src/objects/eagle.cpp \
    src/app_state/menu.cpp \
    src/app_state/scores.cpp

CC = g++
CFLAGS = -c -Wall -std=c++11
LFLAGS = -O

LIBS += -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf


HEADERS += \
    src/app.h \
    src/engine/engine.h \
    src/engine/renderer.h \
    src/engine/sound.h \
    src/app_state/appstate.h \
    src/app_state/game.h \
    src/appconfig.h \
    src/tank.h \
    src/objects/object.h \
    src/objects/tank.h \
    src/engine/spriteconfig.h \
    src/type.h \
    src/objects/objectfactory.h \
    src/objects/enemy.h \
    src/objects/player.h \
    src/objects/bullet.h \
    src/objects/brick.h \
    src/objects/eagle.h \
    src/app_state/menu.h \
    src/app_state/scores.h
