#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include "../data/data.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_pixels.h>

// Forward declarations for SDL-only types we still expose via overloads.
struct SDL_Window;
struct SDL_Color;
typedef struct _TTF_Font TTF_Font;

/**
 * @brief
 * Klasa odpowiada za rysowanie obiektów na ekranie.
 */
class Renderer
{
public:
    Renderer() = default;
    virtual ~Renderer() = default;
    /**
     * Wczytanie tekstury z pliku oraz stworzenie renderera związanego z oknem aplikacji.
     * @param window - wskaźnik na obiekt zawartości okna aplikacji
     */
    virtual void loadTexture(SDL_Window* window) = 0;
    /**
     * Wczytwanie czcionki w trzech różnych rozmiarach.
     */
    virtual void loadFont() = 0;
    /**
     * Czyszczenie bufora ekranu.
     */
    virtual void clear() = 0;
    /**
     * Prezentacja bufora ekranu.
     */
    virtual void flush() = 0;
    /**
     * Przerysowanie fragmentu tekstury na fragment bufora ekranu.
     * @param texture_src - źródłowy prostokąt z tekstury
     * @param window_dest - docelowy prostokąt na buforze ekranu
     */
    virtual void drawObject(const Rect *texture_src, const Rect *window_dest) = 0;

    // Backwards-compatible overloads that accept SDL types and map to project Rect.
    inline void drawObject(const SDL_Rect *texture_src, const SDL_Rect *window_dest)
    {
        if(texture_src == nullptr || window_dest == nullptr) return;
        Rect t = { texture_src->x, texture_src->y, texture_src->w, texture_src->h };
        Rect w = { window_dest->x, window_dest->y, window_dest->w, window_dest->h };
        drawObject(&t, &w);
    }
    /**
     * Ustawienie skali wyświetlanego bufora, tak aby miał zachowane proporcje planszy oraz aby był umiejscowiony w środku okna aplikacji.
     * @param xs - skala pozioma jako stosunek szerokości okna do szerokości mapy
     * @param ys - skala pionowa jako stosunek wysokości okna do wysokości mapy
     * @see AppConfig::map_rect
     */
    virtual void setScale(float xs, float ys) = 0;
    /**
     * Rysowanie tekstu w buforze okna w wybranej pozycji początkowej.
     * @param start - położenie punktu początkowego rysowanego tekstu; ujemna wartości którejś ze wspołrzędnych skutkuje wyśrodkowaniem napisu w tej osi
     * @param text - rysowany tekst
     * @param text_color - kolory rysowanego tekst
     * @param font_size - numer czcionki za pomocą, której będzi rysoweny tekst; dostępne trzy wartośc: 1, 2, 3
     */
    virtual void drawText(const Point* start, std::string text, SDL_Color text_color, int font_size = 1) = 0;

    // Handle nullptr case for Point
    virtual void drawText(std::nullptr_t start, std::string text, SDL_Color text_color, int font_size = 1)
    {
        drawText(static_cast<const Point*>(nullptr), text, text_color, font_size);
    }

    inline void drawText(const SDL_Point* start, std::string text, SDL_Color text_color, int font_size = 1)
    {
        if(start == nullptr)
        {
            drawText(static_cast<const Point*>(nullptr), text, text_color, font_size);
            return;
        }
        Point p = { start->x, start->y };
        drawText(&p, text, text_color, font_size);
    }
    /**
     * Funkcja rysująca prostokątk w buforze okna.
     * @param rect - położneie prostokątku na planszy
     * @param rect_color - kolor prostokątku
     * @param fill - zmienna mówiącza czy prostokąt ma być zamalowany
     */
    virtual void drawRect(const Rect* rect, SDL_Color rect_color, bool fill = false) = 0;

    inline void drawRect(const SDL_Rect* rect, SDL_Color rect_color, bool fill = false)
    {
        if(rect == nullptr) return;
        Rect r = { rect->x, rect->y, rect->w, rect->h };
        drawRect(&r, rect_color, fill);
    }

    virtual void toggleFullscreen(SDL_Window* window) = 0;


private:
    // Concrete implementations may keep their own SDL-specific members.
};

#endif // RENDERER_H
