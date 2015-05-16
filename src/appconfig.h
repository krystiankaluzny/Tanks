#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "objects/player.h"
#include <iostream>
#include <SDL2/SDL_rect.h>
#include <vector>

using namespace std;

/**
 * @brief Klasa zawierająca zbór parametrów aplikacji.
 */
class AppConfig
{
public:
    /**
     * Ścieżka do tekstury obiektów.
     */
    static string texture_path;
    /**
     * Ścieżka do katalogu z poziomami.
     */
    static string levels_path;
    /**
     * Ścieżka do czcionki.
     */
    static string font_name;
    /**
     * Tekst wyświetlany po przegranej.
     */
    static string game_over_text;
    /**
     * Wymiary planszy gry.
     */
    static SDL_Rect map_rect;
    /**
     * Wymiari i położneie względem planszy obszaru statusu gry.
     */
    static SDL_Rect status_rect;
    /**
     * Rozmiar okna aplikacji.
     */
    static SDL_Rect windows_rect;
    /**
     * Rozmiar komórki na planszy.
     */
    static SDL_Rect tile_rect;
    /**
     * Dwie pozycje początkowe graczy.
     */
    static vector<SDL_Point> player_starting_point;
    /**
     * Trzy pozycjie początkowe wrogów.
     */
    static vector<SDL_Point> enemy_starting_point;
    /**
     * Sterowanie czołgami graczy.
     */
    static vector <Player::PlayerKeys> player_keys;
    /**
     * Czas wyświetlanai numeru rundy przy jej rozpocząciu.
     */
    static unsigned level_start_time;
    /**
     * Czas trwania poślizgu na lodzie w milisekundach.
     */
    static unsigned slip_time;
    /**
     * Liczba wrogów do zabicia w czasie jednej rundy.
     */
    static unsigned enemy_start_count;
    /**
     * Czas po jakim na mapie może pojawić się nowy przeciwnik w milisekundach.
     */
    static unsigned enemy_redy_time;
    /**
     * Domyślna maksymalna liczba pocisków ile gracz może wystrzelić.
     */
    static unsigned player_bullet_max_size;
    /**
     * Czas pokazywanie wyników po skończenie odliczania punktów w milisekundach.
     */
    static unsigned score_show_time;
    /**
     * Czas przez jaki bonus jest widoczny na mapie w milisekundach.
     */
    static unsigned bonus_show_time;
    /**
     * Czas działanie osłonki w milisekundach.
     */
    static unsigned tank_shield_time;
    /**
     * Czas przez jaki wrogowei są zamrożeni w milisekundach.
     */
    static unsigned tank_frozen_time;
    /**
     * Czas między zabieciem wszystkich wrogów a przejściem do okna wyników w milisekundach.
     */
    static unsigned level_end_time;
    /**
     * Czas trwania kamiennej osłonki wokół orzełka w milisekundach.
     */
    static unsigned protect_eagle_time;
    /**
     * Czas migania bonusu w milisekundach.
     */
    static unsigned bonus_blink_time;
    /**
     * Minimalny czas między wystrzeleniem pocisków przez gracza w milisekundach.
     */
    static unsigned player_reload_time;
    /**
     * Maksymalna liczba czołgów na mapie w jednej chwili.
     */
    static int enemy_max_count_on_map;
    /**
     * Prędkość poruszania się napsu "GAME OVER".
     */
    static double game_over_entry_speed;
    /**
     * Nominalna prędkość czołgu.
     */
    static double tank_default_speed;
    /**
     * Nominalna prędkość pocisku.
     */
    static double bullet_default_speed;
    /**
     * Zmienna przechowuje informację o tym czy zostało włączone pokazywanie celów przeciwników.
     */
    static bool show_enemy_target;
};

#endif // APPCONFIG_H
