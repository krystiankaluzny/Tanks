#include "sdl_renderer.h"
#include "../../appconfig.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>

SDLRenderer::SDLRenderer()
{
    m_texture = nullptr;
    m_renderer = nullptr;
    m_text_texture = nullptr;
    m_font1 = nullptr;
    m_font2 = nullptr;
    m_font3 = nullptr;
}

SDLRenderer::~SDLRenderer()
{
    if(m_renderer != nullptr)
        SDL_DestroyRenderer(m_renderer);
    if(m_texture != nullptr)
        SDL_DestroyTexture(m_texture);
    if(m_text_texture != nullptr)
        SDL_DestroyTexture(m_text_texture);
    if(m_font1 != nullptr)
        TTF_CloseFont(m_font1);
    if(m_font2 != nullptr)
        TTF_CloseFont(m_font2);
    if(m_font3 != nullptr)
        TTF_CloseFont(m_font3);
}

void SDLRenderer::loadTexture(SDL_Window* window)
{
    SDL_Surface* surface = nullptr;
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    surface = IMG_Load(AppConfig::texture_path.c_str());

    //load surface
    if(surface != nullptr && m_renderer != nullptr)
        m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);

    SDL_FreeSurface(surface);
}

void SDLRenderer::loadFont()
{
    m_font1 = TTF_OpenFont(AppConfig::font_name.c_str(), 28);
    m_font2 = TTF_OpenFont(AppConfig::font_name.c_str(), 14);
    m_font3 = TTF_OpenFont(AppConfig::font_name.c_str(), 10);
}

void SDLRenderer::clear()
{
    SDL_SetRenderDrawColor(m_renderer, 110, 110, 110, 255);
    SDL_RenderClear(m_renderer); //czyścimy tylny bufor
}

void SDLRenderer::flush()
{
    SDL_RenderPresent(m_renderer); //zamieniamy bufory
}

void SDLRenderer::drawObject(const SDL_Rect *texture_src, const SDL_Rect *window_dest)
{
    SDL_RenderCopy(m_renderer, m_texture, texture_src, window_dest); //rysujemy na tylnim buforze
}

void SDLRenderer::setScale(float xs, float ys)
{
    float scale = std::min(xs, ys);
    if(scale < 0.1) return;

    SDL_Rect viewport;
    viewport.x = ((float)AppConfig::windows_rect.w / scale - (AppConfig::map_rect.w + AppConfig::status_rect.w)) / 2.0;
    viewport.y = ((float)AppConfig::windows_rect.h / scale - AppConfig::map_rect.h) / 2.0;
    if(viewport.x < 0) viewport.x = 0;
    if(viewport.y < 0) viewport.y = 0;
    viewport.w = AppConfig::map_rect.w + AppConfig::status_rect.w;
    viewport.h = AppConfig::map_rect.h;

    SDL_RenderSetScale(m_renderer, scale, scale);
    SDL_RenderSetViewport(m_renderer, &viewport);
}

void SDLRenderer::drawText(const SDL_Point* start, std::string text, SDL_Color text_color, int font_size)
{
    if(m_font1 == nullptr || m_font2 == nullptr || m_font3 == nullptr) return;
    if(m_text_texture != nullptr)
        SDL_DestroyTexture(m_text_texture);

    SDL_Surface* text_surface = nullptr;
    if(font_size == 2) text_surface = TTF_RenderText_Solid(m_font2, text.c_str(), text_color);
    else if(font_size == 3) text_surface = TTF_RenderText_Solid(m_font3, text.c_str(), text_color);
    else text_surface = TTF_RenderText_Solid(m_font1, text.c_str(), text_color);

    if(text_surface == nullptr) return;

    m_text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    if(m_text_texture == nullptr) return;

    SDL_Rect window_dest;
    if(start == nullptr)
    {
        window_dest.x = (AppConfig::map_rect.w + AppConfig::status_rect.w - text_surface->w)/2;
        window_dest.y = (AppConfig::map_rect.h - text_surface->h)/2;
    }
    else
    {
        if(start->x < 0) window_dest.x = (AppConfig::map_rect.w + AppConfig::status_rect.w - text_surface->w)/2;
        else window_dest.x = start->x;

        if(start->y < 0) window_dest.y = (AppConfig::map_rect.h - text_surface->h)/2;
        else window_dest.y = start->y;
    }
    window_dest.w = text_surface->w;
    window_dest.h = text_surface->h;

    SDL_RenderCopy(m_renderer, m_text_texture, NULL, &window_dest);
}

void SDLRenderer::drawRect(const SDL_Rect *rect, SDL_Color rect_color, bool fill)
{
    SDL_SetRenderDrawColor(m_renderer, rect_color.r, rect_color.g, rect_color.b, rect_color.a);

    if(fill)
        SDL_RenderFillRect(m_renderer, rect);
    else
        SDL_RenderDrawRects(m_renderer, rect, 1);
}

void SDLRenderer::toggleFullscreen(SDL_Window* window) {
    m_fullscreen = !m_fullscreen;

    if (m_fullscreen)
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    else
        SDL_SetWindowFullscreen(window, 0);
}
