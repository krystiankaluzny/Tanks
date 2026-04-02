#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "engine/data/data.h"
#include "engine/data/keycode.h"
#include <iostream>
#include <vector>

using namespace std;

// Macbook default keyboard does not has a right ctrl key
#if defined(__APPLE__) && defined(__MACH__)
#define P1_FIRE_KEY KeyCode::KEY_RALT
#define P2_FIRE_KEY KeyCode::KEY_LALT
#else
#define P1_FIRE_KEY KeyCode::KEY_RCTRL
#define P2_FIRE_KEY KeyCode::KEY_LCTRL
#endif

namespace AppConfig
{
    inline string texture_path = "texture.png";
    inline string stages_path = "stages/";
    inline string font_path = "kongtext.ttf";

    inline string game_over_text = "GAME OVER";

    inline Size map_size  = {26, 26};
    inline Size tile_size = {16, 16};

    inline Rect map_rect     = {0, 0, map_size.w * tile_size.w, map_size.h * tile_size.h};
    inline Rect status_rect  = {map_rect.w, 0, 3 * tile_size.w, map_rect.h};
    inline Rect windows_rect = {0, 0, map_rect.w + status_rect.w, map_rect.h};

    inline vector<Point> player_starting_point = []
    {
        vector<Point> v;
        v.push_back({8  * tile_size.w, 24 * tile_size.h});
        v.push_back({16 * tile_size.w, 24 * tile_size.h});
        return v;
    }();

    inline vector<Point> enemy_starting_point = []
    {
        vector<Point> v;
        v.push_back({1, 1});
        v.push_back({12 * tile_size.w, 1});
        v.push_back({24 * tile_size.w, 1});
        return v;
    }();

    inline vector<KeyCode> player_1_keys = []
    {
        vector<KeyCode> v;
        v.push_back(KeyCode::KEY_UP);
        v.push_back(KeyCode::KEY_DOWN);
        v.push_back(KeyCode::KEY_LEFT);
        v.push_back(KeyCode::KEY_RIGHT);
        v.push_back(P1_FIRE_KEY);
        return v;
    }();

    inline vector<KeyCode> player_2_keys = []
    {
        vector<KeyCode> v;
        v.push_back(KeyCode::KEY_W);
        v.push_back(KeyCode::KEY_S);
        v.push_back(KeyCode::KEY_A);
        v.push_back(KeyCode::KEY_D);
        v.push_back(P2_FIRE_KEY);
        return v;
    }();

    inline unsigned slip_time = 380;

    inline unsigned enemies_to_kill_total_count = 20;
    inline unsigned enemies_max_count_on_map    = 4;

    inline unsigned stages_count             = 35;
    inline unsigned new_enemy_cooldown       = 500;
    inline unsigned player_bullet_base_count = 2;
    inline unsigned bonus_show_time          = 10000;
    inline unsigned tank_shield_time         = 10000;
    inline unsigned tank_frozen_time         = 8000;
    inline unsigned protect_eagle_time       = 15000;
    inline unsigned bonus_blink_time         = 350;
    inline unsigned player_reload_time       = 120;
    inline double   tank_default_speed       = 0.08;
    inline double   bullet_default_speed     = 0.23;

    namespace Score
    {
        inline unsigned single_count_time = 100;
        inline unsigned idle_time         = 5000;
        inline unsigned high_score_time   = 5200;
    }

    namespace Game
    {
        inline unsigned stage_start_time        = 3200;
        inline unsigned stage_end_time          = 5000;
        inline double   game_over_message_speed = 0.13;
    }
}

#endif // APPCONFIG_H
