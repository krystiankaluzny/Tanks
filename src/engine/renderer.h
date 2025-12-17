#ifndef RENDERER_H
#define RENDERER_H

#include "data/data.h"
#include "data/font_size.h"
#include <string>

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

    /**
     * Draw text at the specified position in the screen buffer.
     * @param start - start location of drawing text. If some coordinate is negative; text will be drawn at the center of axis with negative value
     */
    virtual void drawText(const Point &start, std::string text, Color text_color, FontSize font_size) = 0;

    /**
     * Draw a rectangle at the specified position in the screen buffer.
     */
    virtual void drawRect(const Rect &rect, Color rect_color, bool fill = false) = 0;
};

#endif // RENDERER_H
