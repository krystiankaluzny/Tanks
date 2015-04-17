#include "appconfig.h"

string AppConfig::texture_path = "texture.png";
string AppConfig::levels_path = "levels/";
string AppConfig::font_name = "prstartk.ttf";
string AppConfig::game_over_text = "GAME OVER";
int AppConfig::map_width = 26*16;
int AppConfig::map_height = 26*16;
int AppConfig::windows_width = 26*16;
int AppConfig::windows_height = 26*16;
int AppConfig::player1_starting_point_x = 144;
int AppConfig::player1_starting_point_y = 384;
int AppConfig::tile_width = 16;
int AppConfig::tile_height = 16;
unsigned AppConfig::level_start_time = 2000;
double AppConfig::game_over_entry_speed = 0.13;
