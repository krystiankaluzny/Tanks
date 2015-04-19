#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <iostream>
#include <SDL2/SDL_rect.h>
using namespace std;

class AppConfig
{
public:
    static string texture_path;
    static string levels_path;
    static string font_name;
    static string game_over_text;
    static SDL_Rect map_rect;
    static SDL_Rect status_rect;
    static SDL_Rect windows_rect;
    static SDL_Rect tile_rect;
    static SDL_Point player1_starting_point;
    static unsigned level_start_time;
    static unsigned slip_time;
    static double game_over_entry_speed;
};

#endif // APPCONFIG_H
