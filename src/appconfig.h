#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "objects/player.h"
#include <iostream>
#include <SDL2/SDL_rect.h>
#include <vector>

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
    static vector<SDL_Point> player_starting_point;
    static vector<SDL_Point> enemy_starting_point;
    static vector <Player::PlayerKeys> player_keys;
    static unsigned level_start_time;
    static unsigned slip_time;
    static unsigned enemy_start_count;
    static unsigned enemy_max_count_on_map;
    static double game_over_entry_speed;
    static double tank_default_speed;
    static double bullet_default_speed;
    static bool show_enemy_target;
};

#endif // APPCONFIG_H
