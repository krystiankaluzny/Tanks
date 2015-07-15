#ifndef OBJECT_H
#define OBJECT_H

#include "../engine/engine.h"

/**
 * @brief
 * Klasa bazowa dla obiektów w grze.
 */
class Object
{
public:
    /**
     * Tworzenie obiektu w położeniu (0, 0).
     */
    Object();
    /**
     * Tworzenie obiektu.
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     * @param type - typ obiektu
     */
    Object(double x, double y, SpriteType type);
    /**
     * Tworzenie obiektu.
     * @param x - pozycja początkowa pozioma
     * @param y - pozycja początkowa pionowa
     * @param sprite - animacja obiektu danego typu
     */
    Object(double x, double y, const SpriteData* sprite);
    virtual ~Object();

    /**
     * Rysowanie za pomocą metody @a drawObject z klasy @a Renderer, obiektu z tekstury o współrzędnych src_rect w obszar mapy o wpółrzędnych dest_rect.
     */
    virtual void draw();
    /**
     * Uaktualnienie prostokątan dest_rect na podstawie pozycji obiektu: pos_x, pos_y. Odliczanie czasu wyświetlania jednej klatki animacji i zmiana klatki po odliczeniu opowiedniego czasu.
     * @param dt - czas od ostatniego wywołania funkcji, wykorzystywany do odliczania czasu wyświetlania klatki
     */
    virtual void update(Uint32 dt);

    /**
     * Zmienna mowi czy obiekt ma być usunięty. Jeżeli zmianan jest równa @a true to nie aktualizacja i rysowanie obiektu jest pomijane.
     */
    bool to_erase;
    /**
     * Prostokąt kolizji; może być mniejszy niż wymiary dest_rect.
     */
    SDL_Rect collision_rect;
    /**
     * Pozycja docelowa obiektu na ekranie.
     */
    SDL_Rect dest_rect;
    /**
     * Pozycja na teksturze aktualnie wyświetlanej klatki.
     */
    SDL_Rect src_rect;
    /**
     * Typ obiektu.
     */
    SpriteType type;
    /**
     * Dokładna pozycja pozioma obiektu.
     */
    double pos_x;
    /**
     * Dokładna pozycja pionowa obiektu.
     */
    double pos_y;

protected:
    /**
     * Funkcja zwraca prostokąt przesunięty o wielokrotności rozmiaru prostokąta rect.
     * @param rect - prostokąt bazowy
     * @param x - przesunięcie poziome
     * @param y - przesunięcie pionowe
     * @return przesunięty prostokąt
     */
    SDL_Rect moveRect(const SDL_Rect &rect, int x, int y);

    /**
     * Animacja odpowiadająca danemu typowi obiektu.
     */
    const SpriteData* m_sprite;
    /**
     * Czas wyświetlania obecnej klatki animacji.
     */
    Uint32 m_frame_display_time;
    /**
     * Numer obecnej klatki animacji.
     */
    int m_current_frame;
};

/**
 * Funkcja wyznaczająca częśc wspólną dwuch prostokątków
 * @param rect1
 * @param rect2
 * @return część wspólną, jeśli rect1 i rect2 nie mają części wspólnej prostokąt wyjściowy będzie miał ujemne wymiary
 */
SDL_Rect intersectRect(SDL_Rect* rect1, SDL_Rect* rect2);

#endif // OBJECT_H
