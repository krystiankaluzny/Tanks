#ifndef ENEMY_H
#define ENEMY_H

#include "tank.h"

/**
 * @brief Klasa zajmująca się ruchami wrogich czołgów.
 */
class Enemy : public Tank
{
public:
    /**
     * Tworzenie przeciwnika w pierwszym z położeń wrogów.
     * @see AppConfig::enemy_starting_point
     */
    Enemy();
    /**
     * Tworzenie przeciwnika
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     * @param type - typ czołgu przeciwnika
     */
    Enemy(double x, double y, SpriteType type);

    /**
     * Funkcja rysuje czołg przeciwnika i jeżeli jest ustawiona flaga @a AppConfig::show_enemy_target to rysuję linią łączącą czołg z jego celem.
     */
    void draw();
    /**
     * Funkcja uaktualnia położenie czołgu oraz decyduje o kirunku i czasie kolejnego wystrzału w zależności od typy wroga.
     * @param dt - czas od ostatniego wywołania funkcji
     */
    void update(Uint32 dt);
    /**
     * Zmniejszczenie poziomu pancerza o 1. Jeżeli poziom pancerza dojdze do zera następuje wybuch (zniszczenie) czołgu.
     */
    void destroy();
    /**
     * Funkcja zwraca liczbę punktów jakie dostanie gracz za trafienie danego czołgu wroga.
     * @return punkty
     */
    unsigned scoreForHit();

    /**
     * Pozycja do jakiej kieruje się czołg przeciwnika.
     */
    SDL_Point target_position;

private:
    /**
     * Czas od ostatniej zmiany kierunku.
     */
    Uint32 m_direction_time;
    /**
     * Czas jazdy w danym kirunku. Czas po jakim nastąpi zmiana kierunku.
     */
    Uint32 m_keep_direction_time;

    /**
     * Czas od ostatniej próby wznowienia jazdy
     */
    Uint32 m_speed_time;
    /**
     * Czas po jakim nastąpi kolejne wznowienie jazdy; ustawienie niezerowej prędkości.
     */
    Uint32 m_try_to_go_time;

    /**
     * Czas od ostatniej próby wystrzału pocisku.
     */
    Uint32 m_fire_time;
    /**
     * Czas po jakim nastąpi próba kolejnego wystrzału.
     */
    Uint32 m_reload_time;
};

#endif // ENEMY_H
