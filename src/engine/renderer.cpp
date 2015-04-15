#include "renderer.h"
#include "../appconfig.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

Renderer::Renderer()
{
    m_texture = nullptr;
    m_renderer = nullptr;
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyTexture(m_texture);
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

void Renderer::drawRect(const SDL_Rect *rect, int r, int g, int b)
{
    SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);

    SDL_RenderDrawLine(m_renderer, rect->x, rect->y, rect->x + rect->w, rect->y); //top
    SDL_RenderDrawLine(m_renderer, rect->x, rect->y, rect->x, rect->y + rect->h); //left
    SDL_RenderDrawLine(m_renderer, rect->x + rect->w, rect->y, rect->x + rect->w, rect->y + rect->h); //right
    SDL_RenderDrawLine(m_renderer, rect->x, rect->y + rect->h, rect->x + rect->w, rect->y + rect->h); //bottom
}
