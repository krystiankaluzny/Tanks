#ifndef SDL_EVENT_MAPPER_H
#define SDL_EVENT_MAPPER_H

#include "../data/event.h"
#include <SDL2/SDL.h>

KeyboardEvent mapSDLEventToKeyboardEvent(const SDL_Event &sdl_event)
{
    SDL_Keysym keysym = sdl_event.key.keysym;
    KeyCode keycode;
    unsigned int keymods = 0;

    switch (keysym.sym)
    {
    case SDLK_RETURN:
        keycode = KeyCode::KEY_RETURN;
        break;
    case SDLK_ESCAPE:
        keycode = KeyCode::KEY_ESCAPE;
        break;
    case SDLK_BACKSPACE:
        keycode = KeyCode::KEY_BACKSPACE;
        break;
    case SDLK_TAB:
        keycode = KeyCode::KEY_TAB;
        break;
    case SDLK_SPACE:
        keycode = KeyCode::KEY_SPACE;
        break;
    case SDLK_0:
        keycode = KeyCode::KEY_0;
        break;
    case SDLK_1:
        keycode = KeyCode::KEY_1;
        break;
    case SDLK_2:
        keycode = KeyCode::KEY_2;
        break;
    case SDLK_3:
        keycode = KeyCode::KEY_3;
        break;
    case SDLK_4:
        keycode = KeyCode::KEY_4;
        break;
    case SDLK_5:
        keycode = KeyCode::KEY_5;
        break;
    case SDLK_6:
        keycode = KeyCode::KEY_6;
        break;
    case SDLK_7:
        keycode = KeyCode::KEY_7;
        break;
    case SDLK_8:
        keycode = KeyCode::KEY_8;
        break;
    case SDLK_9:
        keycode = KeyCode::KEY_9;
        break;
    case SDLK_a:
        keycode = KeyCode::KEY_A;
        break;
    case SDLK_b:
        keycode = KeyCode::KEY_B;
        break;
    case SDLK_c:
        keycode = KeyCode::KEY_C;
        break;
    case SDLK_d:
        keycode = KeyCode::KEY_D;
        break;
    case SDLK_e:
        keycode = KeyCode::KEY_E;
        break;
    case SDLK_f:
        keycode = KeyCode::KEY_F;
        break;
    case SDLK_g:
        keycode = KeyCode::KEY_G;
        break;
    case SDLK_h:
        keycode = KeyCode::KEY_H;
        break;
    case SDLK_i:
        keycode = KeyCode::KEY_I;
        break;
    case SDLK_j:
        keycode = KeyCode::KEY_J;
        break;
    case SDLK_k:
        keycode = KeyCode::KEY_K;
        break;
    case SDLK_l:
        keycode = KeyCode::KEY_L;
        break;
    case SDLK_m:
        keycode = KeyCode::KEY_M;
        break;
    case SDLK_n:
        keycode = KeyCode::KEY_N;
        break;
    case SDLK_o:
        keycode = KeyCode::KEY_O;
        break;
    case SDLK_p:
        keycode = KeyCode::KEY_P;
        break;
    case SDLK_q:
        keycode = KeyCode::KEY_Q;
        break;
    case SDLK_r:
        keycode = KeyCode::KEY_R;
        break;
    case SDLK_s:
        keycode = KeyCode::KEY_S;
        break;
    case SDLK_t:
        keycode = KeyCode::KEY_T;
        break;
    case SDLK_u:
        keycode = KeyCode::KEY_U;
        break;
    case SDLK_v:
        keycode = KeyCode::KEY_V;
        break;
    case SDLK_w:
        keycode = KeyCode::KEY_W;
        break;
    case SDLK_x:
        keycode = KeyCode::KEY_X;
        break;
    case SDLK_y:
        keycode = KeyCode::KEY_Y;
        break;
    case SDLK_z:
        keycode = KeyCode::KEY_Z;
        break;
    case SDLK_UP:
        keycode = KeyCode::KEY_UP;
        break;
    case SDLK_DOWN:
        keycode = KeyCode::KEY_DOWN;
        break;
    case SDLK_LEFT:
        keycode = KeyCode::KEY_LEFT;
        break;
    case SDLK_RIGHT:
        keycode = KeyCode::KEY_RIGHT;
        break;
    case SDLK_LSHIFT:
        keycode = KeyCode::KEY_LSHIFT;
        break;
    case SDLK_RSHIFT:
        keycode = KeyCode::KEY_RSHIFT;
        break;
    case SDLK_LCTRL:
        keycode = KeyCode::KEY_LCTRL;
        break;
    case SDLK_RCTRL:
        keycode = KeyCode::KEY_RCTRL;
        break;
    case SDLK_LALT:
        keycode = KeyCode::KEY_LALT;
        break;
    case SDLK_RALT:
        keycode = KeyCode::KEY_RALT;
        break;
    case SDLK_F1:
        keycode = KeyCode::KEY_F1;
        break;
    case SDLK_F2:
        keycode = KeyCode::KEY_F2;
        break;
    case SDLK_F3:
        keycode = KeyCode::KEY_F3;
        break;
    case SDLK_F4:
        keycode = KeyCode::KEY_F4;
        break;
    case SDLK_F5:
        keycode = KeyCode::KEY_F5;
        break;
    case SDLK_F6:
        keycode = KeyCode::KEY_F6;
        break;
    case SDLK_F7:
        keycode = KeyCode::KEY_F7;
        break;
    case SDLK_F8:
        keycode = KeyCode::KEY_F8;
        break;
    case SDLK_F9:
        keycode = KeyCode::KEY_F9;
        break;
    case SDLK_F10:
        keycode = KeyCode::KEY_F10;
        break;
    case SDLK_F11:
        keycode = KeyCode::KEY_F11;
        break;
    case SDLK_F12:
        keycode = KeyCode::KEY_F12;
        break;
    default:
        keycode = KeyCode::KEY_UNKNOWN;
        break;
    }

    if (keysym.mod & KMOD_LSHIFT)
        keymods |= KeyMod::MOD_LSHIFT;
    if (keysym.mod & KMOD_RSHIFT)
        keymods |= KeyMod::MOD_RSHIFT;

    if (keysym.mod & KMOD_LCTRL)
        keymods |= KeyMod::MOD_LCTRL;
    if (keysym.mod & KMOD_RCTRL)
        keymods |= KeyMod::MOD_RCTRL;

    if (keysym.mod & KMOD_LALT)
        keymods |= KeyMod::MOD_LALT;
    if (keysym.mod & KMOD_RALT)
        keymods |= KeyMod::MOD_RALT;

    if (keysym.mod & KMOD_LGUI)
        keymods |= KeyMod::MOD_LGUI;
    if (keysym.mod & KMOD_RGUI)
        keymods |= KeyMod::MOD_RGUI;

    if (keysym.mod & KMOD_NUM)
        keymods |= KeyMod::MOD_NUM;
    if (keysym.mod & KMOD_CAPS)
        keymods |= KeyMod::MOD_CAPS;
    if (keysym.mod & KMOD_MODE)
        keymods |= KeyMod::MOD_MODE;
    if (keysym.mod & KMOD_SCROLL)
        keymods |= KeyMod::MOD_SCROLL;

    return KeyboardEvent(keycode, keymods);
}

WindowEvent mapSDLEventToWindowEvent(const SDL_Event &sdl_event)
{
    SDL_WindowEvent sdl_window_event = sdl_event.window;
    int data1 = sdl_window_event.data1;
    int data2 = sdl_window_event.data2;

    switch (sdl_window_event.event)
    {
    case SDL_WINDOWEVENT_SHOWN:
        return WindowEvent(WindowEvent::SHOWN, data1, data2);
    case SDL_WINDOWEVENT_HIDDEN:
        return WindowEvent(WindowEvent::HIDDEN, data1, data2);
    case SDL_WINDOWEVENT_EXPOSED:
        return WindowEvent(WindowEvent::EXPOSED, data1, data2);
    case SDL_WINDOWEVENT_MOVED:
        return WindowEvent(WindowEvent::MOVED, data1, data2);
    case SDL_WINDOWEVENT_RESIZED:
        return WindowEvent(WindowEvent::RESIZED, data1, data2);
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        return WindowEvent(WindowEvent::SIZE_CHANGED, data1, data2);
    case SDL_WINDOWEVENT_MINIMIZED:
        return WindowEvent(WindowEvent::MINIMIZED, data1, data2);
    case SDL_WINDOWEVENT_MAXIMIZED:
        return WindowEvent(WindowEvent::MAXIMIZED, data1, data2);
    case SDL_WINDOWEVENT_RESTORED:
        return WindowEvent(WindowEvent::RESTORED, data1, data2);
    case SDL_WINDOWEVENT_ENTER:
        return WindowEvent(WindowEvent::ENTER, data1, data2);
    case SDL_WINDOWEVENT_LEAVE:
        return WindowEvent(WindowEvent::LEAVE, data1, data2);
    case SDL_WINDOWEVENT_FOCUS_GAINED:
        return WindowEvent(WindowEvent::FOCUS_GAINED, data1, data2);
    case SDL_WINDOWEVENT_FOCUS_LOST:
        return WindowEvent(WindowEvent::FOCUS_LOST, data1, data2);
    case SDL_WINDOWEVENT_CLOSE:
        return WindowEvent(WindowEvent::CLOSE, data1, data2);
    default:
        return WindowEvent(WindowEvent::UNKNOWN, data1, data2);
    }

    return WindowEvent(WindowEvent::UNKNOWN, data1, data2);
}

Event mapSDLEventToEngineEvent(const SDL_Event &sdl_event)
{
    switch (sdl_event.type)
    {
    case SDL_KEYDOWN:
        return mapSDLEventToKeyboardEvent(sdl_event);
    case SDL_WINDOWEVENT:
        return mapSDLEventToWindowEvent(sdl_event);
    case SDL_QUIT:
        return QuitEvent();
    default:
        return UnknownEvent();
    }
    return UnknownEvent();
}

#endif // SDL_EVENT_MAPPER_H