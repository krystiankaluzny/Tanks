#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include "data/data.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_pixels.h>


class Renderer
{
public:
    Renderer() = default;
    virtual ~Renderer() = default;

    /**
     * Clear the screen buffer.
     */
    virtual void clear() = 0;
    /**
     * Flush the screen buffer to the display.
     */
    virtual void flush() = 0;

    /**
     * Draw an object from the texture to the screen buffer.
     */
    virtual void drawObject(const Rect &texture_src, const Rect &window_dest) = 0;

    //TODO doc
    //TODO add font size enum
    /**
     * Draw text at the specified position in the screen buffer.
     * @param start - położenie punktu początkowego rysowanego tekstu; ujemna wartości którejś ze wspołrzędnych skutkuje wyśrodkowaniem napisu w tej osi
     * @param text - rysowany tekst
     * @param text_color - kolory rysowanego tekst
     * @param font_size - numer czcionki za pomocą, której będzi rysoweny tekst; dostępne trzy wartośc: 1, 2, 3
     */
    virtual void drawText(const Point &start, std::string text, Color text_color, int font_size = 1) = 0;

    /**
     * Draw a rectangle at the specified position in the screen buffer.
     */
    virtual void drawRect(const Rect &rect, Color rect_color, bool fill = false) = 0;
};

#endif // RENDERER_H
