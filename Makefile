BIN = bin
CC = C:\MinGW\bin\mingw32-g++.exe
CFLAGS = -c -Wall -std=c++11
CONFIG = console
DEPENDPATH = SDL/i686-w64-mingw32/include
HEADERS =  src/app.h  src/engine/engine.h  src/engine/renderer.h  src/engine/sound.h  src/app_state/appstate.h  src/app_state/game.h  src/appconfig.h  src/tank.h  src/objects/object.h  src/objects/tank.h  src/engine/spriteconfig.h  src/type.h  src/objects/objectfactory.h  src/objects/enemy.h  src/objects/player.h  src/objects/bullet.h  src/objects/brick.h  src/objects/eagle.h
INCLUDEPATH = -ISDL/i686-w64-mingw32/include
LFLAGS = -mwindows -O
LIBS = -LSDL/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
PROJECT_NAME = Tanks
SOURCES = src/main.cpp src/app.cpp  src/engine/engine.cpp  src/engine/renderer.cpp  src/engine/sound.cpp  src/app_state/game.cpp  src/appconfig.cpp  src/objects/object.cpp  src/objects/tank.cpp  src/engine/spriteconfig.cpp  src/objects/objectfactory.cpp  src/objects/enemy.cpp  src/objects/player.cpp  src/objects/bullet.cpp  src/objects/brick.cpp  src/objects/eagle.cpp
SRC = src
TARGET = Tanks
TEMPLATE = app
OBJS = src_main.o src_app.o src_engine_engine.o src_engine_renderer.o src_engine_sound.o src_app_state_game.o src_appconfig.o src_objects_object.o src_objects_tank.o src_engine_spriteconfig.o src_objects_objectfactory.o src_objects_enemy.o src_objects_player.o src_objects_bullet.o src_objects_brick.o src_objects_eagle.o

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDEPATH) $(LIBSPATH) $(LIBS) $(LFLAGS) -o $(BIN)/$(PROJECT_NAME)

src_main.o: src/main.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/main.cpp -o src_main.o

src_app.o: src/app.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/app.cpp -o src_app.o

src_engine_engine.o: src/engine/engine.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/engine/engine.cpp -o src_engine_engine.o

src_engine_renderer.o: src/engine/renderer.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/engine/renderer.cpp -o src_engine_renderer.o

src_engine_sound.o: src/engine/sound.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/engine/sound.cpp -o src_engine_sound.o

src_app_state_game.o: src/app_state/game.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/app_state/game.cpp -o src_app_state_game.o

src_appconfig.o: src/appconfig.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/appconfig.cpp -o src_appconfig.o

src_objects_object.o: src/objects/object.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/objects/object.cpp -o src_objects_object.o

src_objects_tank.o: src/objects/tank.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/objects/tank.cpp -o src_objects_tank.o

src_engine_spriteconfig.o: src/engine/spriteconfig.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/engine/spriteconfig.cpp -o src_engine_spriteconfig.o

src_objects_objectfactory.o: src/objects/objectfactory.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/objects/objectfactory.cpp -o src_objects_objectfactory.o

src_objects_enemy.o: src/objects/enemy.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/objects/enemy.cpp -o src_objects_enemy.o

src_objects_player.o: src/objects/player.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/objects/player.cpp -o src_objects_player.o

src_objects_bullet.o: src/objects/bullet.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/objects/bullet.cpp -o src_objects_bullet.o

src_objects_brick.o: src/objects/brick.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/objects/brick.cpp -o src_objects_brick.o

src_objects_eagle.o: src/objects/eagle.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/objects/eagle.cpp -o src_objects_eagle.o


clean:
	rm -r -f *.o
	rm -r -f $(BIN)/*.exe
