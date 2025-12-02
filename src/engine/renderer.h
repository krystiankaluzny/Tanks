#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include "data/data.h"
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
    virtual void loadTexture(SDL_Window *window) = 0;
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
    virtual void drawObject(const Rect &texture_src, const Rect &window_dest) = 0;

    /**
     * Rysowanie tekstu w buforze okna w wybranej pozycji początkowej.
     * @param start - położenie punktu początkowego rysowanego tekstu; ujemna wartości którejś ze wspołrzędnych skutkuje wyśrodkowaniem napisu w tej osi
     * @param text - rysowany tekst
     * @param text_color - kolory rysowanego tekst
     * @param font_size - numer czcionki za pomocą, której będzi rysoweny tekst; dostępne trzy wartośc: 1, 2, 3
     */
    virtual void drawText(const Point &start, std::string text, Color text_color, int font_size = 1) = 0;

    /**
     * Funkcja rysująca prostokątk w buforze okna.
     * @param rect - położneie prostokątku na planszy
     * @param rect_color - kolor prostokątku
     * @param fill - zmienna mówiącza czy prostokąt ma być zamalowany
     */
    virtual void drawRect(const Rect &rect, Color rect_color, bool fill = false) = 0;

private:
    // Concrete implementations may keep their own SDL-specific members.
};

#endif // RENDERER_H
