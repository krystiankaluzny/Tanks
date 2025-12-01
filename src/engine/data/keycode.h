#ifndef ENGINE_DATA_KEYCODE_H
#define ENGINE_DATA_KEYCODE_H

enum KeyCode {
    // Special keys
    KEY_UNKNOWN = 0,
    KEY_RETURN = 13,
    KEY_ESCAPE = 27,
    KEY_BACKSPACE = 8,
    KEY_TAB = 9,
    KEY_SPACE = 32,
        
    // Number keys
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    
    // Letter keys
    KEY_A = 97,
    KEY_B = 98,
    KEY_C = 99,
    KEY_D = 100,
    KEY_E = 101,
    KEY_F = 102,
    KEY_G = 103,
    KEY_H = 104,
    KEY_I = 105,
    KEY_J = 106,
    KEY_K = 107,
    KEY_L = 108,
    KEY_M = 109,
    KEY_N = 110,
    KEY_O = 111,
    KEY_P = 112,
    KEY_Q = 113,
    KEY_R = 114,
    KEY_S = 115,
    KEY_T = 116,
    KEY_U = 117,
    KEY_V = 118,
    KEY_W = 119,
    KEY_X = 120,
    KEY_Y = 121,
    KEY_Z = 122,

    // Function keys
    KEY_F1 = 1001,
    KEY_F2 = 1002,
    KEY_F3 = 1003,
    KEY_F4 = 1004,
    KEY_F5 = 1005,
    KEY_F6 = 1006,
    KEY_F7 = 1007,
    KEY_F8 = 1008,
    KEY_F9 = 1009,
    KEY_F10 = 1010,
    KEY_F11 = 1011,
    KEY_F12 = 1012,
    
    // Arrow keys
    KEY_UP = 2001,
    KEY_DOWN = 2002,
    KEY_LEFT = 2003,
    KEY_RIGHT = 2004,
    
    // Modifier keys
    KEY_LSHIFT = 3001,
    KEY_RSHIFT = 3002,
    KEY_LCTRL = 3003,
    KEY_RCTRL = 3004,
    KEY_LALT = 3005,
    KEY_RALT = 3006,
};


enum KeyMod {
    MOD_NONE = 0x0000,
    MOD_LSHIFT = 0x0001,
    MOD_RSHIFT = 0x0002,
    MOD_LCTRL = 0x0040,
    MOD_RCTRL = 0x0080,
    MOD_LALT = 0x0100,
    MOD_RALT = 0x0200,
    MOD_LGUI = 0x0400,
    MOD_RGUI = 0x0800,
    MOD_NUM = 0x1000,
    MOD_CAPS = 0x2000,
    MOD_MODE = 0x4000,
    MOD_SCROLL = 0x8000,

    MOD_CTRL = MOD_LCTRL | MOD_RCTRL,
    MOD_SHIFT = MOD_LSHIFT | MOD_RSHIFT,
    MOD_ALT = MOD_LALT | MOD_RALT,
    MOD_GUI = MOD_LGUI | MOD_RGUI
};


#endif // ENGINE_DATA_KEYCODE_H