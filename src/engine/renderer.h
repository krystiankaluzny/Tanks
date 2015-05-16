#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

/**
 * @brief
 * Klasa odpowiada za rysowanie obiektów na ekranie.
 */
class Renderer
{
public:
    Renderer();
    ~Renderer();
    /**
     * Wczytanie tekstury z pliku oraz stworzenie renderera związanego z oknem aplikacji.
     * @param window - wskaźnik na obiekt zawartości okna aplikacji
     */
    void loadTexture(SDL_Window* window);
    /**
     * Wczytwanie czcionki w trzech różnych rozmiarach.
     */
    void loadFont();
    /**
     * Czyszczenie bufora ekranu.
     */
    void clear();
    /**
     * Prezentacja bufora ekranu.
     */
    void flush();
    /**
     * Przerysowanie fragmentu tekstury na fragment bufora ekranu.
     * @param texture_src - źródłowy prostokąt z tekstury
     * @param window_dest - docelowy prostokąt na buforze ekranu
     */
    void drawObject(const SDL_Rect *texture_src, const SDL_Rect *window_dest);
    /**
     * Ustawienie skali wyświetlanego bufora, tak aby miał zachowane proporcje planszy oraz aby był umiejscowiony w środku okna aplikacji.
     * @param xs - skala pozioma jako stosunek szerokości okna do szerokości mapy
     * @param ys - skala pionowa jako stosunek wysokości okna do wysokości mapy
     * @see AppConfig::map_rect
     */
    void setScale(float xs, float ys);
    /**
     * Rysowanie tekstu w buforze okna w wybranej pozycji początkowej.
     * @param start - położenie punktu początkowego rysowanego tekstu; ujemna wartości którejś ze wspołrzędnych skutkuje wyśrodkowaniem napisu w tej osi
     * @param text - rysowany tekst
     * @param text_color - kolory rysowanego tekst
     * @param font_size - numer czcionki za pomocą, której będzi rysoweny tekst; dostępne trzy wartośc: 1, 2, 3
     */
    void drawText(const SDL_Point* start, std::string text, SDL_Color text_color, int font_size = 1);
    /**
     * Funkcja rysująca prostokątk w buforze okna.
     * @param rect - położneie prostokątku na planszy
     * @param rect_color - kolor prostokątku
     * @param fill - zmienna mówiącza czy prostokąt ma być zamalowany
     */
    void drawRect(const SDL_Rect* rect, SDL_Color rect_color, bool fill = false);

private:
    /**
     * Wskaźnik na obiekt związany z buforem okna.
     */
    SDL_Renderer* m_renderer;
    /**
     * Wskaźnik na teksturę zawierającą wszystkie widoczne elementy gry.
     */
    SDL_Texture* m_texture;
    /**
     * Wskaźnik na teksturę pomocniczą przy rysowaniu tekstu na ekranie.
     */
    SDL_Texture* m_text_texture;
    /**
     * Czcionka o rozmiarze 28.
     */
    TTF_Font* m_font1;
    /**
     * Czcionka o rozmiarze 14.
     */
    TTF_Font* m_font2;
    /**
     * Czcionka o rozmiarze 10.
     */
    TTF_Font* m_font3;
};

#endif // RENDERER_H
