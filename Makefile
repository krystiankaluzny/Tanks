PROJECT_NAME = Tanks

BUILD = build
BIN = $(BUILD)/bin
RESOURCES_DIR = resources

ifeq ($(OS),Windows_NT) 
    CC = mingw32-g++.exe
    INCLUDEPATH = 
    LFLAGS = -mwindows -O
    LIBS = -L$(RESOURCES_DIR)/SDL/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
    RESOURCES = SDL/i686-w64-mingw32/bin/*.dll dll/*.dll font/prstartk.ttf png/texture.png levels
else
    CC = g++
    INCLUDEPATH = -ISDL/i686-w64-mingw32/include
    LFLAGS = -O
    LIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
    RESOURCES = font/prstartk.ttf png/texture.png levels
endif

CFLAGS = -c -Wall -std=c++11

MODULES = engine app_state objects
SRC_DIRS = src $(addprefix src/,$(MODULES))
BUILD_DIRS = $(BUILD) $(BIN) $(addprefix $(BUILD)/,$(MODULES))

SOURCES = $(foreach sdir,$(SRC_DIRS),$(wildcard $(sdir)/*.cpp))
OBJS = $(patsubst src/%.cpp,$(BUILD)/%.o,$(SOURCES))

vpath %.cpp $(SRC_DIRS)

all: print $(BUILD_DIRS) $(RESOURCES) compile

print:
	@echo OS: $(OS)
	@echo MODULES: $(MODULES)
	@echo SRC_DIRS: $(SRC_DIRS)
	@echo BUILD_DIRS: $(BUILD_DIRS)
	@echo SOURCES: $(SOURCES)
	@echo OBJS: $(OBJS)
	@echo 

checkdirs: $(BUILD_DIRS)

$(BUILD_DIRS):
	mkdir -p $@

compile: $(OBJS)
	$(CC) $(OBJS) $(INCLUDEPATH) $(LIBSPATH) $(LIBS) $(LFLAGS) -o $(BIN)/$(PROJECT_NAME)

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) $< -o $@

$(RESOURCES):
	cp -R $(RESOURCES_DIR)/$@ $(BIN)

clean:
	rm -rf $(BUILD_DIRS)
	rm -rf doc

doc:
	doxywizard Doxyfile
	doxygen