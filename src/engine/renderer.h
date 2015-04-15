#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void loadTexture(SDL_Window* window); //wczytuje teksture

    void clear();
    void flush();
    void drawObject(const SDL_Rect *texture_src, const SDL_Rect *window_dest);
    void setScale(float xs, float ys);

    void drawRect(const SDL_Rect* rect, int r, int g, int b);

private:
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture; //tekstura zawierająca wszystkie postacie
};

#endif // RENDERER_H
