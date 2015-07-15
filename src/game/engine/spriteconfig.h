#ifndef SPRITECONFIG_H
#define SPRITECONFIG_H

#include "../type.h"
#include <map>
#include <SDL2/SDL.h>

/**
 * @brief
 * Struktura przechowująca informacjie danym typie animacji obiektu.
 */
struct SpriteData
{
    SpriteData() { rect.x = 0; rect.y = 0; rect.w = 0; rect.h = 0; frames_count = 0; frame_duration = 0; loop = false;}
    SpriteData(int x, int y, int w, int h, int fc, int fd, bool l)
        { rect.x = x; rect.y = y; rect.w = w; rect.h = h; frames_count = fc; frame_duration = fd; loop = l;}

    /**
     * Pozycja i wymiary pierwszej klatki animacji
     */
    SDL_Rect rect;
    /**
     * Liczba klatek w animacji.
     */
    int frames_count;
    /**
     * Czas wyświetlania jednej klatki milisekundach.
     */
    unsigned frame_duration;
    /**
     * Zmienna określająca czy animacja jest zapętlona.
     */
    bool loop;
};

/**
 * @brief
 * Klasa przechowuje informacjie o wszystkich typach animacji w grze.
 */
class SpriteConfig
{
public:
    /**
     * W konstruktorze wykonywane są dodawania kolejnych animacji funkcją @a SpriteConfig::insert.
     */
    SpriteConfig();
    /**
     * Pobranie wybranego typu animacji.
     * @param sp - szukany typ animacji
     * @return animację danego typu
     */
    const SpriteData* getSpriteData(SpriteType sp) const;
private:
    /**
     * Kontener przechowujący wszystkie typy animacji.
     */
    std::map<SpriteType, SpriteData> m_configs;
    /**
     * Funkcja używana przy dodawaniu nowego typu animacji.
     * @param st - typ animacji
     * @param x - połoźenie poziome pierwszej klatki animacji w teksturze
     * @param y - połoźenie pionowe pierwszej klatki animacji w teksturze
     * @param w - szerokości klatek animacji
     * @param h - wysokości klatek animacji
     * @param fc - liczba klatek w animacji
     * @param fd - czas wyświetlania jednej klatki
     * @param l - zapętlenie animacji
     */
    void insert(SpriteType st , int x, int y, int w, int h, int fc, int fd, bool l);
};

#endif // SPRITECONFIG_H
