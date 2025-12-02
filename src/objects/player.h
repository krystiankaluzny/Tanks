#ifndef PLAYER_H
#define PLAYER_H

#include "tank.h"
#include <vector>
/**
 * @brief Klasa odpowiadająca czołgom graczy.
 */
class Player : public Tank
{
public:
    /**
     * Tworzenie czołgu gracza
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     * @param type - typ gracza
     */
    Player(double x, double y, SpriteType type, std::vector<KeyCode> control_keys);

    void handleKeyboardEvent(const KeyboardEvent &ev);

    /**
     * Funkcaj odpowiada za zmianę animacji czołgu gracza oraz za sprawdzeni stanu wciśniętych klawiszy i reakcja na te klawisze, które sterują czołgiem gracza.
     * @param dt - czas od ostatwniego wywołania funkcji, wykorzystywany przy zmianie animacji
     */
    void update(Uint32 dt) override;

    /**
     * Funkcja odpowiada za odjęcie życia, wyczyszczenie wszystkich flag i włączenie animacji powstawania czołgu.
     */
    void respawn() override;
    /**
     * Funkcja odpowiada za włączenie animacji wybuchu czołgu jeżeli czołgu nie miał osłonki, łódki lub trzech gwiazdek.
     */
    void destroy() override;
    /**
     * Funkcja odpowiada za stworzenie pocisku jeżeli jeszcze nie stworzono maksymalnej ich ilości,
     * nadaniu mu większej szybkości jeżeli gracz ma przynajmniej jedną gwiazdkę oraz dodaniu zwiększonych obrażeni jeżeli gracz ma trzy gwiazdki.
     * @return wskaźnik na utworzony pocisk, jeżeli nie stworzono pocisku zwraca @a nullptr
     */
    Bullet* fire() override;

    /**
     * Funkcja zmienia liczbę aktualnie posiadanych gwiazdek. Przy niezerowej liczbie gwiazdek zwiększana jest domyślna prędkość czołgu,
     * a dla liczby gwiazdek większej od 1 oraz dla każdego dodatniego @a c zwiększana jest maksymalna liczba pocisków.
     * @param c - zmiana liczby gwiazdek, może być ujemna
     */
    void changeStarCountBy(int c);

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

    struct KeyState {
        KeyCode key;
        bool pressed;
    };

    KeyState m_key_state_up;
    KeyState m_key_state_down;
    KeyState m_key_state_left;
    KeyState m_key_state_right;
    KeyState m_key_state_fire;
};

#endif // PLAYER_H
