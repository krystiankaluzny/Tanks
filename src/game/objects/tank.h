#ifndef TANK_H
#define TANK_H

#include "object.h"
#include "bullet.h"
#include "../type.h"

#include <vector>

typedef unsigned TankStateFlags;

/**
 * @brief
 * Klasa zajmująca się podstawową mechaniką czołgów: jazda, strzał.
 */
class Tank : public Object
{
public:
    /**
     * Tworzenie czołgu w pierwszym z położeń wrogów.
     * @see AppConfig::enemy_starting_point
     */
    Tank();
    /**
     * Tworzenie czołgu
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     * @param type - typ czołgu
     */
    Tank(double x, double y, SpriteType type);
    virtual ~Tank();

    /**
     * Funkcja rysuje obrazek czołgu, w razie potrzeby rysuje osłonkę i łódkę.
     * Wywołuje rysowanie pocisków.
     */
    void draw();
    /**
     * Funkcja odpowiada za zmianę położenia czołgu, uaktualnienie położenia prostokątów dest_rect i collision_rect, położenia osłonek i łodzi, animację czołgu,
     * wywołanie aktualizacji pocisku oraz usuwanie zniszczonych pocisków. Funkcja odlicza czas posiadania osłonki i zamrożenia i wyłącza te flagi.
     * @param dt - czas od ostatniego wywołania funkcji, wykorzystywany przy zmianie animacji
     */
    void update(Uint32 dt);
    /**
     * Funkcja odpowiada za stworzenie pocisku jeżeli jeszcze nie stworzono maksymalnej ich ilości.
     * @return wskaźnik na utworzony pocisk, jeżeli nie stworzono pocisku zwraca @a nullptr
     */
    virtual Bullet* fire();
    /**
     * Funkcja zwraca prostokąt kolizji jaki byłby w następnej klatce przy założeniu prędkości i kierunku takiej jaka jest obecnie.
     * @param dt - przewidywany czas wyliczania następnej klatki
     * @return następny prostokąt kolizji
     */
    SDL_Rect nextCollisionRect(Uint32 dt);
    /**
     * Funkcja ustawia następny kierunek ruchu z uwzględnieniem poślizgu na lodzie. Podczas zmiany kierunku następuje dopasowanie czołgu do wielokrotności wymiarów komórki planszy @a AppConfig::tile_rect.
     * @param d - nowy kierunek
     */
    void setDirection(Direction d);
    /**
     * Funkcja zatrzymuje czołg.
     * @param intersect_rect - obszar kolizji
     */
    void collide(SDL_Rect &intersect_rect);
    /**
     * Funkcja odpowiada za wyczyszczenie wszystkich flag i włączenie animacji powstawania czołgu.
     */
    virtual void respawn();
    /**
     * Funkcja odpowiada za włączenie animacji wybuchu czołgu.
     */
    virtual void destroy();
    /**
     * Ustawienie wybranej flagi.
     * @param flag
     */
    void setFlag(TankStateFlag flag);
    /**
     * Wyczyszczenie wybranej flagi.
     * @param flag
     */
    void clearFlag(TankStateFlag flag);
    /**
     * Sprawdzenie czy wybrana flaga jest ustawiona.
     * @param flag
     * @return @a true jeżeli flaga jest ustawiona w przeciwnym wypadku @a false
     */
    bool testFlag(TankStateFlag flag);

    /**
     * Domyślna prędkość danego czołgu. Może być różna dla różnych typów czołgów lub może być zmieniona po wzięcu bonusu przez gracza.
     */
    double default_speed;
    /**
     * Aktualna prędkość czołgu.
     */
    double speed;
    /**
     * Zmienna przechowuję informację czy czołg jest obecnie zatrzymany.
     */
    bool stop;
    /**
     * Zmeinna przechowuje aktualny kierunek jazdy czołgu.
     */
    Direction direction;
    /**
     * Kontener z wystrzelonymi pociskami czołgu.
     */
    std::vector<Bullet*> bullets;
    /**
     * Liczba żyć gracza lub numer poziomu pancerza wrogiego czołgu.
     */
    int lives_count;

protected:
    /**
     * Flagi jakie ma aktualnie czołg.
     */
    TankStateFlags m_flags;
    /**
     * Czas od wystąpienia poślizgu.
     */
    Sint32 m_slip_time;
    /**
     * Odpowiada zwrotowi czołgu w poślizgu i może być różna od kierunku przemieszczania się czołgu na lodzie.
     */
    Direction new_direction;
    /**
     * Maksymalna liczba pocisków jakie może wystrzelić czołg.
     */
    unsigned m_bullet_max_size;

    /**
     * Wskaźnik na osłonkę czołgu. Jeśli czołg nie ma osłonki zmienna ma wartośc nullptr;
     */
    Object* m_shield;
    /**
     * Wskaźnik na łódkę, którą może mieć czołg. Jeśli czołg nie ma łódki zmienna ma wartośc nullptr;
     */
    Object* m_boat;
    /**
     * Czas od zdobycia osłonki.
     */
    Uint32 m_shield_time;
    /**
     * Czas od zamrożenia czoałgu.
     */
    Uint32 m_frozen_time;
};

#endif // TANK_H
