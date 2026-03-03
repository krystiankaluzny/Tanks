PROJECT_NAME = Tanks

SRC = src
BUILD = build
BIN = $(BUILD)/bin
RESOURCES_DIR = resources

ifeq ($(OS),Windows_NT)
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		SDL_ARCH=x86_64-w64-mingw32
	else
		SDL_ARCH=i686-w64-mingw32
	endif

	SDL_MAIN=SDL/SDL2-2.32.4/$(SDL_ARCH)
	SDL_IMAGE=SDL/SDL2_image-2.8.8/$(SDL_ARCH)
	SDL_TTF=SDL/SDL2_ttf-2.24.0/$(SDL_ARCH)
	SDL_MIXER=SDL/SDL2_mixer-2.8.1/$(SDL_ARCH)

	ifneq ($(strip $(MSYSTEM_PREFIX)),)
		CC = $(MSYSTEM_PREFIX)/bin/g++.exe
		INCLUDEPATH = -I$(RESOURCES_DIR)/$(SDL_MAIN)/include -I$(RESOURCES_DIR)/$(SDL_IMAGE)/include -I$(RESOURCES_DIR)/$(SDL_TTF)/include -I$(RESOURCES_DIR)/$(SDL_MIXER)/include
		LFLAGS = -mwindows -O
		CFLAGS = -c -Wall
		LIBS = -L$(RESOURCES_DIR)/$(SDL_MAIN)/lib -L$(RESOURCES_DIR)/$(SDL_IMAGE)/lib -L$(RESOURCES_DIR)/$(SDL_TTF)/lib -L$(RESOURCES_DIR)/$(SDL_MIXER)/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
		APP_RESOURCES = $(SDL_MAIN)/bin/*.dll $(SDL_IMAGE)/bin/*.dll $(SDL_TTF)/bin/*.dll $(SDL_MIXER)/bin/*.dll dll/*.dll font/kongtext.ttf textures/texture.png levels sounds
		RESOURCES = $(APP_RESOURCES)
	else
		CC = $(MINGW_HOME)/bin/mingw32-g++.exe
		INCLUDEPATH = -I$(RESOURCES_DIR)/$(SDL_MAIN)/include -I$(RESOURCES_DIR)/$(SDL_IMAGE)/include -I$(RESOURCES_DIR)/$(SDL_TTF)/include -I$(RESOURCES_DIR)/$(SDL_MIXER)/include
		LFLAGS = -mwindows -O
		CFLAGS = -c -Wall
		LIBS = -L$(RESOURCES_DIR)/$(SDL_MAIN)/lib -L$(RESOURCES_DIR)/$(SDL_IMAGE)/lib -L$(RESOURCES_DIR)/$(SDL_TTF)/lib -L$(RESOURCES_DIR)/$(SDL_MIXER)/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
		APP_RESOURCES = $(SDL_MAIN)/bin/*.dll $(SDL_IMAGE)/bin/*.dll $(SDL_TTF)/bin/*.dll $(SDL_MIXER)/bin/*.dll dll/*.dll font/kongtext.ttf textures/texture.png levels sounds
		RESOURCES = $(APP_RESOURCES) mingw_resources
	endif
else
	UNAME_S := $(shell uname -s)
	UNAME_M := $(shell uname -m)

	ifeq ($(UNAME_S),Darwin)
		ifeq ($(UNAME_M),arm64)
			# Mac M1 (Apple Silicon)
			CC = g++
			INCLUDEPATH = -I/opt/homebrew/include
			LIBSPATH = -L/opt/homebrew/lib
		else
			# Mac x86_64 (Intel)
			CC = g++
			INCLUDEPATH = -I/usr/local/include
			LIBSPATH = -L/usr/local/lib
		endif
		LFLAGS = -O
		CFLAGS = -c -Wall -Wno-narrowing -std=c++17
		LIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
		APP_RESOURCES = font/kongtext.ttf textures/texture.png levels sounds
		RESOURCES = $(APP_RESOURCES)
	else
		CC = g++
		INCLUDEPATH =
		LFLAGS = -O
		CFLAGS = -c -Wall -Wno-narrowing -std=c++17
		LIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
		APP_RESOURCES = font/kongtext.ttf textures/texture.png levels sounds
		RESOURCES = $(APP_RESOURCES)
	endif
endif


MODULES = engine engine/sdl app_state objects
SRC_SUBDIRS = $(shell cd $(SRC) && find * -type d)
SRC_DIRS = $(SRC) $(addprefix $(SRC)/,$(SRC_SUBDIRS))
BUILD_DIRS = $(BUILD) $(BIN) $(addprefix $(BUILD)/,$(SRC_SUBDIRS))

SOURCES = $(foreach sdir,$(SRC_DIRS),$(wildcard $(sdir)/*.cpp))
OBJS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SOURCES))

VERSION := $(shell git describe --tags --abbrev=0 | sed -Ee 's/^v|-.*//')

vpath %.cpp $(SRC_DIRS)

all: print $(BUILD_DIRS) $(RESOURCES) compile

print:
	@echo
	@echo VERSION: $(VERSION)
	@echo OS: $(OS)
	@echo SRC: $(SRC)
	@echo SRC_SUBDIRS: $(SRC_SUBDIRS)
	@echo SRC_DIRS: $(SRC_DIRS)
	@echo BUILD_DIRS: $(BUILD_DIRS)
	@echo SOURCES: $(SOURCES)
	@echo RESOURCES: $(RESOURCES)
	@echo OBJS: $(OBJS)
	@echo INCLUDEPATH: $(INCLUDEPATH)
	@echo LIBSPATH: $(LIBSPATH)
	@echo LIBS: $(LIBS)
	@echo LFLAGS: $(LFLAGS)
	@echo

$(BUILD_DIRS):
	mkdir -p $@

compile: $(OBJS)
	$(CC) $(OBJS) $(INCLUDEPATH) $(LIBSPATH) $(LIBS) $(LFLAGS) -o $(BIN)/$(PROJECT_NAME)

build/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDEPATH) $< -o $@

$(APP_RESOURCES):
	cp -R $(RESOURCES_DIR)/$@ $(BIN)

ifeq ($(OS),Windows_NT)

mingw_resources:
	cp $(MINGW_HOME)/bin/libstdc++-6.dll $(BIN)
	cp $(MINGW_HOME)/bin/libgcc_s_dw2-1.dll $(BIN)

endif

clean:
	rm -rf $(BUILD)
	rm -rf doc

run: all
	cd build/bin && ./Tanks


bump-minor:
	git tag -a $(shell echo $(VERSION) | awk -F. -v OFS=. '{$$2++;print}') -m "Bump minor version to $(shell echo $(VERSION) | awk -F. -v OFS=. '{$$2++;print}')"
	git push --tags

bump-patch:
	git tag -a $(shell echo $(VERSION) | awk -F. -v OFS=. '{$$3++;print}') -m "Bump patch version to $(shell echo $(VERSION) | awk -F. -v OFS=. '{$$3++;print}')"
	git push --tags