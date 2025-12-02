#include "appconfig.h"

string AppConfig::texture_path = "texture.png";
string AppConfig::levels_path = "levels/";
string AppConfig::font_name = "prstartk.ttf";
string AppConfig::game_over_text = "GAME OVER";
Rect AppConfig::map_rect = {0, 0, 26*16, 26*16};
Rect AppConfig::status_rect = {26*16, 0, 3*16, AppConfig::map_rect.h};
Rect AppConfig::windows_rect = {0, 0, AppConfig::map_rect.w + AppConfig::status_rect.w, AppConfig::map_rect.h};
Rect AppConfig::tile_rect = {0, 0, 16, 16};
// Macbook default keyboard does not has a right ctrl key
#if defined(__APPLE__) && defined(__MACH__)
    #define P1_FIRE_KEY KeyCode::KEY_RALT
    #define P2_FIRE_KEY KeyCode::KEY_LALT
#else
    #define P1_FIRE_KEY KeyCode::KEY_RCTRL
    #define P2_FIRE_KEY KeyCode::KEY_LCTRL
#endif

vector<Point> AppConfig::player_starting_point =
[]{
    vector<Point> v;
    v.push_back({128, 384});
    v.push_back({256, 384});
    return v;
}();
vector<Point> AppConfig::enemy_starting_point =
[]{
    vector<Point> v;
    v.push_back({1, 1});
    v.push_back({192, 1});
    v.push_back({384, 1});
    return v;
}();
vector<KeyCode> AppConfig::player_1_keys =
[]{
    vector<KeyCode> v;
    v.push_back(KeyCode::KEY_UP);
    v.push_back(KeyCode::KEY_DOWN);
    v.push_back(KeyCode::KEY_LEFT);
    v.push_back(KeyCode::KEY_RIGHT);
    v.push_back(P1_FIRE_KEY);
    return v;
}();
vector<KeyCode> AppConfig::player_2_keys =
[]{
    vector<KeyCode> v;
    v.push_back(KeyCode::KEY_W);
    v.push_back(KeyCode::KEY_S);
    v.push_back(KeyCode::KEY_A);
    v.push_back(KeyCode::KEY_D);
    v.push_back(P2_FIRE_KEY);
    return v;
}();
unsigned AppConfig::level_start_time = 2000;
unsigned AppConfig::slip_time = 380;
unsigned AppConfig::enemy_start_count = 20;
unsigned AppConfig::enemy_redy_time = 500;
unsigned AppConfig::player_bullet_max_size = 2;
unsigned AppConfig::score_show_time = 3000;
unsigned AppConfig::bonus_show_time = 10000;
unsigned AppConfig::tank_shield_time = 10000;
unsigned AppConfig::tank_frozen_time = 8000;
unsigned AppConfig::level_end_time = 5000;
unsigned AppConfig::protect_eagle_time = 15000;
unsigned AppConfig::bonus_blink_time = 350;
unsigned AppConfig::player_reload_time = 120;
int AppConfig::enemy_max_count_on_map = 4;
double AppConfig::game_over_entry_speed = 0.13;
double AppConfig::tank_default_speed = 0.08;
double AppConfig::bullet_default_speed = 0.23;
bool AppConfig::show_enemy_target = false;
