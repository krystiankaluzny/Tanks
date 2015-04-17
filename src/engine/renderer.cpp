#include "renderer.h"
#include "../appconfig.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

Renderer::Renderer()
{
    m_texture = nullptr;
    m_renderer = nullptr;
    m_text_texture = nullptr;
    m_font = nullptr;
}

Renderer::~Renderer()
{
    if(m_renderer != nullptr)
        SDL_DestroyRenderer(m_renderer);
    if(m_texture != nullptr)
        SDL_DestroyTexture(m_texture);
    if(m_text_texture != nullptr)
        SDL_DestroyTexture(m_text_texture);
    if(m_font != nullptr)
        TTF_CloseFont(m_font);
}

void Renderer::loadTexture(SDL_Window* window)
{
    SDL_Surface* surface = nullptr;
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    surface = IMG_Load(AppConfig::texture_path.c_str());

    //load surface
    if(surface != nullptr && m_renderer != nullptr)
    {
        m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
        if(m_texture == nullptr)
        {
            //zgłoszenie błędu
        }
    }
    else
    {
        //zgłoszenie błądu
    }

    SDL_FreeSurface(surface);
}

void Renderer::loadFont()
{
    m_font = TTF_OpenFont(AppConfig::font_name.c_str(), 28);
}

void Renderer::clear()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer); //czyścimy tylny bufor
}

void Renderer::flush()
{
    SDL_RenderPresent(m_renderer); //zamieniamy bufory
}

void Renderer::drawObject(const SDL_Rect *texture_src, const SDL_Rect *window_dest)
{

    SDL_RenderCopy(m_renderer, m_texture, texture_src, window_dest); //rysujemy na tylnim buforze
}

void Renderer::setScale(float xs, float ys)
{
    SDL_RenderSetScale(m_renderer, xs, ys);
}

void Renderer::drawText(const SDL_Point* start, string text, SDL_Color text_color)
{
    if(m_font == nullptr) return;
    if(m_text_texture != nullptr)
        SDL_DestroyTexture(m_text_texture);

    SDL_Surface* text_surface = TTF_RenderText_Solid(m_font, text.c_str(), text_color);
    if(text_surface == nullptr) return;

    m_text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    if(m_text_texture == nullptr) return;

    SDL_Rect window_dest;
    if(start == nullptr)
    {
        window_dest.x = (AppConfig::windows_width - text_surface->w)/2;
        window_dest.y = (AppConfig::windows_height - text_surface->h)/2;
    }
    else
    {
        if(start->x < 0) window_dest.x = (AppConfig::windows_width - text_surface->w)/2;
        else window_dest.x = start->x;

        if(start->y < 0) window_dest.y = (AppConfig::windows_height - text_surface->h)/2;
        else window_dest.y = start->y;
    }
    window_dest.w = text_surface->w;
    window_dest.h = text_surface->h;

    SDL_RenderCopy(m_renderer, m_text_texture, NULL, &window_dest);
}

void Renderer::drawRect(const SDL_Rect *rect, SDL_Color text_color)
{
    SDL_SetRenderDrawColor(m_renderer, text_color.r, text_color.g, text_color.b, text_color.a);

    SDL_RenderDrawLine(m_renderer, rect->x, rect->y, rect->x + rect->w, rect->y); //top
    SDL_RenderDrawLine(m_renderer, rect->x, rect->y, rect->x, rect->y + rect->h); //left
    SDL_RenderDrawLine(m_renderer, rect->x + rect->w, rect->y, rect->x + rect->w, rect->y + rect->h); //right
    SDL_RenderDrawLine(m_renderer, rect->x, rect->y + rect->h, rect->x + rect->w, rect->y + rect->h); //bottom
}
