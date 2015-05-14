#ifndef PLAYER_H
#define PLAYER_H

#include "tank.h"

class Player : public Tank
{
public:
    /**
     * Struktura przechowująca klawiszę odpowiadające sterowaniem czołgiem gracza.
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

    Player();
    Player(double x, double y, SpriteType type);


    /**
     * Funkcaj odpowiada za zmianę animacji czołgu gracza oraz za sprawdzeni stanu wciśniętych klawiszy i reakcja na te klawisze, które sterują czołgiem gracza.
     * @param dt - czas od ostatwniego wywołania funkcji, wykorzystywany przy zmianie animacji
     */
    void update(Uint32 dt);
    /**
     * @brief respawn
     */
    void respawn();
    void destroy();
    Bullet* fire();
    void changeStarCountBy(int c);

    PlayerKeys player_keys;
    unsigned score;
    int star_count;

private:
    Uint32 m_fire_time;
};

#endif // PLAYER_H
