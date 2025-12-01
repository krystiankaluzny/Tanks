#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include "../renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class SDLRenderer : public Renderer
{
public:
    SDLRenderer(Size viewport_base_size);
    ~SDLRenderer();

    void loadTexture(SDL_Window *window) override;
    void loadFont() override;
    void clear() override;
    void flush() override;
    void drawObject(const Rect *texture_src, const Rect *window_dest) override;
    void drawText(const Point *start, std::string text, SDL_Color text_color, int font_size = 1) override;
    void drawRect(const Rect *rect, SDL_Color rect_color, bool fill = false) override;

    
    void toggleFullscreen(SDL_Window *window);
    void setViewportForWindowSize(Size window_size);

private:
    Size m_viewport_base_size;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;
    SDL_Texture *m_text_texture;
    TTF_Font *m_font1;
    TTF_Font *m_font2;
    TTF_Font *m_font3;
    bool m_fullscreen;
};

#endif // SDL_RENDERER_H
