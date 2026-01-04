#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "engine/data/data.h"
#include "engine/data/keycode.h"
#include <iostream>
#include <vector>

using namespace std;


class AppConfig
{
public:

    static string texture_path;
    static string levels_path;
    static string font_path;

    static string game_over_text;

    static Size map_size; // Number of tiles in the map (width, height)
    static Size tile_size; // Size of a single tile in pixels


    static Rect map_rect;
    static Rect status_rect;
    static Rect windows_rect;
    

    static vector<Point> player_starting_point;
    static vector<Point> enemy_starting_point;

    static vector<KeyCode> player_1_keys;
    static vector<KeyCode> player_2_keys;


    static unsigned level_start_time;
    static unsigned slip_time;


    static unsigned enemies_to_kill_total_count;
    static unsigned enemies_max_count_on_map;

    static unsigned levels_count;
    static unsigned new_enemy_cooldown;
    static unsigned player_bullet_max_count;
    static unsigned score_show_time;
    static unsigned score_count_time;
    static unsigned bonus_show_time;
    static unsigned tank_shield_time;
    static unsigned tank_frozen_time;
    static unsigned level_end_time;
    static unsigned protect_eagle_time;
    static unsigned bonus_blink_time;
    static unsigned player_reload_time;

    static double game_over_message_speed;
    static double tank_default_speed;
    static double bullet_default_speed;

};

#endif // APPCONFIG_H
