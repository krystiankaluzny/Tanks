#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <iostream>
using namespace std;

class AppConfig
{
public:
    static string texture_path;
    static string levels_path;
    static string font_name;
    static string game_over_text;
    static int map_width;
    static int map_height;
    static int windows_width;
    static int windows_height;
    static int player1_starting_point_x;
    static int player1_starting_point_y;
    static int tile_width;
    static int tile_height;
    static unsigned level_start_time; //czas wyświetlania "STAGE X"
    static double game_over_entry_speed;
};

#endif // APPCONFIG_H
