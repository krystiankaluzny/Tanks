#include "appconfig.h"

string AppConfig::texture_path = "texture.png";
string AppConfig::levels_path = "levels/";
string AppConfig::font_name = "prstartk.ttf";
string AppConfig::game_over_text = "GAME OVER";
SDL_Rect AppConfig::map_rect = {0, 0, 26*16, 26*16};
SDL_Rect AppConfig::status_rect = {0, 0, 4*16, AppConfig::map_rect.h};
SDL_Rect AppConfig::windows_rect = {0, 0, AppConfig::map_rect.w + AppConfig::status_rect.w, AppConfig::map_rect.h};
SDL_Rect AppConfig::tile_rect = {0, 0, 16, 16};

vector<SDL_Point> AppConfig::player_starting_point =
        []{vector<SDL_Point> v; v.push_back({128, 384});v.push_back({256, 384}); return v;}();
vector<SDL_Point> AppConfig::enemy_starting_point =
        []{vector<SDL_Point> v; v.push_back({1, 1});v.push_back({192, 1});v.push_back({384, 1}); return v;}();

unsigned AppConfig::level_start_time = 200; // 2000;
unsigned AppConfig::slip_time = 380;
double AppConfig::game_over_entry_speed = 0.13;
double AppConfig::tank_default_speed = 0.08;
double AppConfig::bullet_default_speed = 0.23;
