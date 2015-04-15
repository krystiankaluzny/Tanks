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
    src/objects/brick.cpp

CC = C:\MinGW\bin\mingw32-g++.exe
CFLAGS = -c -Wall -std=c++11
LFLAGS = -mwindows -O

LIBS += -L$$PWD/SDL/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

DEPENDPATH += $$PWD/SDL/i686-w64-mingw32/include
INCLUDEPATH += $$PWD/SDL/i686-w64-mingw32/include

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
    src/objects/brick.h


OTHER_FILES += \
    font/arialbd.ttf \
    font/prstartk.ttf \
    png/graphics.png \
    png/texture.png \
    SDL/i686-w64-mingw32/include/SDL2/begin_code.h \
    SDL/i686-w64-mingw32/include/SDL2/close_code.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_assert.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_atomic.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_audio.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_bits.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_blendmode.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_clipboard.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_config.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_cpuinfo.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_endian.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_error.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_events.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_filesystem.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_gamecontroller.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_gesture.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_haptic.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_hints.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_image.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_joystick.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_keyboard.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_keycode.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_loadso.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_log.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_main.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_messagebox.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_mouse.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_mutex.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_name.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_opengl.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_opengles.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_opengles2.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_pixels.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_platform.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_power.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_quit.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_rect.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_render.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_revision.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_rwops.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_scancode.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_shape.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_stdinc.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_surface.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_system.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_syswm.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_assert.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_common.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_compare.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_crc32.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_font.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_fuzzer.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_harness.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_images.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_log.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_md5.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_test_random.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_thread.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_timer.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_touch.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_ttf.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_types.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_version.h \
    SDL/i686-w64-mingw32/include/SDL2/SDL_video.h \
    SDL/i686-w64-mingw32/bin/libfreetype-6.dll \
    SDL/i686-w64-mingw32/bin/libjpeg-9.dll \
    SDL/i686-w64-mingw32/bin/libpng16-16.dll \
    SDL/i686-w64-mingw32/bin/libtiff-5.dll \
    SDL/i686-w64-mingw32/bin/libwebp-4.dll \
    SDL/i686-w64-mingw32/bin/SDL2.dll \
    SDL/i686-w64-mingw32/bin/SDL2_image.dll \
    SDL/i686-w64-mingw32/bin/SDL2_ttf.dll \
    SDL/i686-w64-mingw32/bin/zlib1.dll \
    SDL/i686-w64-mingw32/bin/sdl2-config \
    SDL/i686-w64-mingw32/lib/libSDL2.a \
    SDL/i686-w64-mingw32/lib/libSDL2.dll.a \
    SDL/i686-w64-mingw32/lib/libSDL2_image.a \
    SDL/i686-w64-mingw32/lib/libSDL2_image.dll.a \
    SDL/i686-w64-mingw32/lib/libSDL2_test.a \
    SDL/i686-w64-mingw32/lib/libSDL2_ttf.a \
    SDL/i686-w64-mingw32/lib/libSDL2_ttf.dll.a \
    SDL/i686-w64-mingw32/lib/libSDL2main.a \
    SDL/i686-w64-mingw32/lib/libSDL2.la \
    SDL/i686-w64-mingw32/lib/libSDL2_image.la \
    SDL/i686-w64-mingw32/lib/libSDL2_ttf.la \
    SDL/i686-w64-mingw32/lib/pkgconfig/sdl2.pc \
    SDL/i686-w64-mingw32/lib/pkgconfig/SDL2_image.pc \
    SDL/i686-w64-mingw32/lib/pkgconfig/SDL2_ttf.pc \
    SDL/i686-w64-mingw32/share/aclocal/sdl2.m4 \
    bin/arialbd.ttf \
    bin/libgcc_s_dw2-1.dll \
    bin/libpng16-16.dll \
    bin/libstdc++-6.dll \
    bin/libtiff-5.dll \
    bin/libwebp-4.dll \
    bin/prstartk.ttf \
    bin/SDL2.dll \
    bin/SDL2_image.dll \
    bin/Tanks.exe \
    bin/zlib1.dll \
    bin/texture.png \
    bin/levels/1 \
    bin/levels/1a \
    bin/levels/2 \
    bin/levels/3 \
    bin/levels/4 \
    bin/levels/5 \
    bin/levels/6 \
    bin/levels/7 \
    bin/levels/8 \
    bin/levels/9 \
    bin/levels/10 \
    bin/levels/11 \
    bin/levels/12 \
    bin/levels/13 \
    bin/levels/14 \
    bin/levels/15 \
    bin/levels/16 \
    bin/levels/17 \
    bin/levels/18 \
    bin/levels/19 \
    bin/levels/20 \
    bin/levels/21 \
    bin/levels/22 \
    bin/levels/23 \
    bin/levels/24 \
    bin/levels/25 \
    bin/levels/26 \
    bin/levels/27 \
    bin/levels/28 \
    bin/levels/29 \
    bin/levels/30 \
    bin/levels/31 \
    bin/levels/32 \
    bin/levels/33 \
    bin/levels/34 \
    bin/levels/35

