# requremanets libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev


BIN = bin
# CC = C:\MinGW\bin\mingw32-g++.exe
CC = g++
CFLAGS = -c -Wall -std=c++11
# INCLUDEPATH = -ISDL/i686-w64-mingw32/include
# LFLAGS = -mwindows -O
LFLAGS = -O
# LIBS = -LSDL/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
LIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
PROJECT_NAME = Tanks

MODULES = engine app_state objects
SRC_DIRS = src $(addprefix src/,$(MODULES))
BUILD_DIRS = build $(addprefix build/,$(MODULES))

SOURCES = $(foreach sdir,$(SRC_DIRS),$(wildcard $(sdir)/*.cpp))
OBJS = $(patsubst src/%.cpp,build/%.o,$(SOURCES))

vpath %.cpp $(SRC_DIRS)

all: print checkdirs build

checkdirs: $(BUILD_DIRS)

$(BUILD_DIRS):
	mkdir -p $@

build: $(OBJS)
	$(CC) $(OBJS) $(INCLUDEPATH) $(LIBSPATH) $(LIBS) $(LFLAGS) -o build/$(PROJECT_NAME)

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) $< -o $@

print:
	@echo MODULES $(MODULES)
	@echo SRC_DIRS $(SRC_DIRS)
	@echo BUILD_DIRS $(BUILD_DIRS)
	@echo SOURCES $(SOURCES)
	@echo OBJS $(OBJS)

clean:
	@rm -rf $(BUILD_DIRS)