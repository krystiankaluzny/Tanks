#LFLAGS = -mwindows -O
BIN = bin
CC = C:\MinGW\bin\mingw32-g++.exe
CFLAGS = -c -Wall -std=c++11
CONFIG = console
DEPENDPATH = SDL/i686-w64-mingw32/include
HEADERS = src/app.h src/appconfig.h  src/appthread.h  src/type.h  src/game/game.h  src/game/engine/engine.h  src/game/engine/renderer.h  src/game/engine/spriteconfig.h  src/game/game_state/battle.h  src/game/game_state/gamestate.h  src/game/game_state/menu.h  src/game/game_state/scores.h  src/game/game_state/server.h  src/game/objects/bonus.h  src/game/objects/brick.h  src/game/objects/bullet.h  src/game/objects/eagle.h  src/game/objects/enemy.h  src/game/objects/object.h  src/game/objects/player.h  src/game/objects/tank.h  src/network/servertcp.h  src/event/event.h  src/scopelock.h  src/event/eventstore.h  src/shareddata.h  src/network/network.h  src/network/tcpconnection.h  src/network/clienttcp.h  src/game/game_state/client.h  src/game/handler/handler.h  src/game/game_state/networkbattle.h
INCLUDEPATH = -ISDL/i686-w64-mingw32/include
LIBS = -LSDL/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lws2_32
PROJECT_NAME = Tanks
SOURCES = src/app.cpp src/appconfig.cpp  src/main.cpp  src/game/game.cpp  src/game/engine/engine.cpp  src/game/engine/renderer.cpp  src/game/engine/spriteconfig.cpp  src/game/game_state/battle.cpp  src/game/game_state/menu.cpp  src/game/game_state/scores.cpp  src/game/game_state/server.cpp  src/game/objects/bonus.cpp  src/game/objects/brick.cpp  src/game/objects/bullet.cpp  src/game/objects/eagle.cpp  src/game/objects/enemy.cpp  src/game/objects/object.cpp  src/game/objects/player.cpp  src/game/objects/tank.cpp  src/network/servertcp.cpp  src/event/event.cpp  src/scopelock.cpp  src/event/eventstore.cpp  src/shareddata.cpp  src/network/network.cpp  src/appthread.cpp  src/network/clienttcp.cpp  src/network/tcpconnection.cpp  src/game/game_state/client.cpp  src/game/handler/handler.cpp  src/game/game_state/networkbattle.cpp
SRC = src
TARGET = Tanks
TEMPLATE = app
OBJS = src_app.o src_appconfig.o src_main.o src_game_game.o src_game_engine_engine.o src_game_engine_renderer.o src_game_engine_spriteconfig.o src_game_game_state_battle.o src_game_game_state_menu.o src_game_game_state_scores.o src_game_game_state_server.o src_game_objects_bonus.o src_game_objects_brick.o src_game_objects_bullet.o src_game_objects_eagle.o src_game_objects_enemy.o src_game_objects_object.o src_game_objects_player.o src_game_objects_tank.o src_network_servertcp.o src_event_event.o src_scopelock.o src_event_eventstore.o src_shareddata.o src_network_network.o src_appthread.o src_network_clienttcp.o src_network_tcpconnection.o src_game_game_state_client.o src_game_handler_handler.o src_game_game_state_networkbattle.o

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDEPATH) $(LIBSPATH) $(LIBS) $(LFLAGS) -o $(BIN)/$(PROJECT_NAME)

src_app.o: src/app.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/app.cpp -o src_app.o

src_appconfig.o: src/appconfig.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/appconfig.cpp -o src_appconfig.o

src_main.o: src/main.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/main.cpp -o src_main.o

src_game_game.o: src/game/game.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/game.cpp -o src_game_game.o

src_game_engine_engine.o: src/game/engine/engine.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/engine/engine.cpp -o src_game_engine_engine.o

src_game_engine_renderer.o: src/game/engine/renderer.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/engine/renderer.cpp -o src_game_engine_renderer.o

src_game_engine_spriteconfig.o: src/game/engine/spriteconfig.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/engine/spriteconfig.cpp -o src_game_engine_spriteconfig.o

src_game_game_state_battle.o: src/game/game_state/battle.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/game_state/battle.cpp -o src_game_game_state_battle.o

src_game_game_state_menu.o: src/game/game_state/menu.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/game_state/menu.cpp -o src_game_game_state_menu.o

src_game_game_state_scores.o: src/game/game_state/scores.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/game_state/scores.cpp -o src_game_game_state_scores.o

src_game_game_state_server.o: src/game/game_state/server.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/game_state/server.cpp -o src_game_game_state_server.o

src_game_objects_bonus.o: src/game/objects/bonus.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/objects/bonus.cpp -o src_game_objects_bonus.o

src_game_objects_brick.o: src/game/objects/brick.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/objects/brick.cpp -o src_game_objects_brick.o

src_game_objects_bullet.o: src/game/objects/bullet.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/objects/bullet.cpp -o src_game_objects_bullet.o

src_game_objects_eagle.o: src/game/objects/eagle.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/objects/eagle.cpp -o src_game_objects_eagle.o

src_game_objects_enemy.o: src/game/objects/enemy.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/objects/enemy.cpp -o src_game_objects_enemy.o

src_game_objects_object.o: src/game/objects/object.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/objects/object.cpp -o src_game_objects_object.o

src_game_objects_player.o: src/game/objects/player.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/objects/player.cpp -o src_game_objects_player.o

src_game_objects_tank.o: src/game/objects/tank.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/objects/tank.cpp -o src_game_objects_tank.o

src_network_servertcp.o: src/network/servertcp.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/network/servertcp.cpp -o src_network_servertcp.o

src_event_event.o: src/event/event.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/event/event.cpp -o src_event_event.o

src_scopelock.o: src/scopelock.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/scopelock.cpp -o src_scopelock.o

src_event_eventstore.o: src/event/eventstore.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/event/eventstore.cpp -o src_event_eventstore.o

src_shareddata.o: src/shareddata.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/shareddata.cpp -o src_shareddata.o

src_network_network.o: src/network/network.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/network/network.cpp -o src_network_network.o

src_appthread.o: src/appthread.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/appthread.cpp -o src_appthread.o

src_network_clienttcp.o: src/network/clienttcp.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/network/clienttcp.cpp -o src_network_clienttcp.o

src_network_tcpconnection.o: src/network/tcpconnection.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/network/tcpconnection.cpp -o src_network_tcpconnection.o

src_game_game_state_client.o: src/game/game_state/client.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/game_state/client.cpp -o src_game_game_state_client.o

src_game_handler_handler.o: src/game/handler/handler.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/handler/handler.cpp -o src_game_handler_handler.o

src_game_game_state_networkbattle.o: src/game/game_state/networkbattle.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) src/game/game_state/networkbattle.cpp -o src_game_game_state_networkbattle.o


clean:
	rm -r -f *.o
	rm -r -f $(BIN)/*.exe
