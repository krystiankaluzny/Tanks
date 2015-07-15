#ifndef PLAYER_H
#define PLAYER_H

#include "tank.h"

/**
 * @brief Klasa odpowiadająca czołgom graczy.
 */
class Player : public Tank
{
public:

    /**
     * @brief Struktura przechowująca klawiszę odpowiadające sterowaniem czołgiem gracza.
     */
    struct PlayerKeys
    {
        PlayerKeys(): up(SDL_SCANCODE_UNKNOWN), down(SDL_SCANCODE_UNKNOWN), left(SDL_SCANCODE_UNKNOWN), right(SDL_SCANCODE_UNKNOWN), fire(SDL_SCANCODE_UNKNOWN) {}
        PlayerKeys(SDL_Scancode u, SDL_Scancode d, SDL_Scancode l, SDL_Scancode r, SDL_Scancode f): up(u), down(d), left(l), right(r), fire(f) {}
        /**
         * Klawisz odpowiadający jeździe w górę.
         */
        SDL_Scancode up;
        /**
         * Klawisz odpowiadający jeździe w dół.
         */
        SDL_Scancode down;
        /**
         * Klawisz odpowiadający jeździe w lewo.
         */
        SDL_Scancode left;
        /**
         * Klawisz odpowiadający jeździe w prawo.
         */
        SDL_Scancode right;
        /**
         * Klawisz odpowiadający wystrzałowi pocisku.
         */
        SDL_Scancode fire;
    };

    /**
     * Tworzenie gracza w pierwszym z położeń graczy.
     * @see AppConfig::player_starting_point
     */
    Player();
    /**
     * Tworzenie czołgu gracza
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     * @param type - typ gracza
     */
    Player(double x, double y, SpriteType type);


    /**
     * Funkcaj odpowiada za zmianę animacji czołgu gracza oraz za sprawdzeni stanu wciśniętych klawiszy i reakcja na te klawisze, które sterują czołgiem gracza.
     * @param dt - czas od ostatwniego wywołania funkcji, wykorzystywany przy zmianie animacji
     */
    void update(Uint32 dt);
    /**
     * Funkcja odpowiada za odjęcie życia, wyczyszczenie wszystkich flag i włączenie animacji powstawania czołgu.
     */
    void respawn();
    /**
     * Funkcja odpowiada za włączenie animacji wybuchu czołgu jeżeli czołgu nie miał osłonki, łódki lub trzech gwiazdek.
     */
    void destroy();
    /**
     * Funkcja odpowiada za stworzenie pocisku jeżeli jeszcze nie stworzono maksymalnej ich ilości,
     * nadaniu mu większej szybkości jeżeli gracz ma przynajmniej jedną gwiazdkę oraz dodaniu zwiększonych obrażeni jeżeli gracz ma trzy gwiazdki.
     * @return wskaźnik na utworzony pocisk, jeżeli nie stworzono pocisku zwraca @a nullptr
     */
    Bullet* fire();

    /**
     * Funkcja zmienia liczbę aktualnie posiadanych gwiazdek. Przy niezerowej liczbie gwiazdek zwiększana jest domyślna prędkość czołgu,
     * a dla liczby gwiazdek większej od 1 oraz dla każdego dodatniego @a c zwiększana jest maksymalna liczba pocisków.
     * @param c - zmiana liczby gwiazdek, może być ujemna
     */
    void changeStarCountBy(int c);

    /**
     * Klawiszcze sterujące ruchami aktualngo gracza.
     */
    PlayerKeys player_keys;
    /**
     * Aktualnie posiadane punkty przez gracza.
     */
    unsigned score;

private:
    /**
     * Aktualnie posiadana liczba gwiazdek; może się zawierać w przedziale [0, 3].
     */
    int star_count;
    /**
     * Czas jaki minął od ostatnego wystrzału pocisku.
     */
    Uint32 m_fire_time;
};

#endif // PLAYER_H
