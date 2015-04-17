#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void loadTexture(SDL_Window* window); //wczytuje teksture
    void loadFont();
    void clear();
    void flush();

    void drawObject(const SDL_Rect *texture_src, const SDL_Rect *window_dest);
    void setScale(float xs, float ys);
    void drawText(const SDL_Point* start, std::string text, SDL_Color text_color);
    void drawRect(const SDL_Rect* rect, SDL_Color text_color);

private:
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture; //tekstura zawierająca wszystkie postacie
    SDL_Texture* m_text_texture;
    TTF_Font* m_font;
};

#endif // RENDERER_H
