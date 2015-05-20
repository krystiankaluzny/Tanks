TEMPLATE = app
TARGET = Tanks
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/app.cpp \
    src/app_state/game.cpp \
    src/appconfig.cpp \
    src/app_state/menu.cpp \
    src/app_state/scores.cpp

CC = C:\MinGW\bin\mingw32-g++.exe
CFLAGS = -c -Wall -std=c++11
LFLAGS = -mwindows -O

LIBS += -L$$PWD/SDL/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

DEPENDPATH += $$PWD/SDL/i686-w64-mingw32/include
INCLUDEPATH += $$PWD/SDL/i686-w64-mingw32/include

HEADERS += \
    src/app.h \
    src/app_state/appstate.h \
    src/app_state/game.h \
    src/appconfig.h \
    src/objects/object.h \
    src/objects/tank.h \
    src/type.h \
    src/app_state/menu.h \
    src/app_state/scores.h
