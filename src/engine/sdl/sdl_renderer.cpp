#include "sdl_renderer.h"
#include "../../appconfig.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>

SDLRenderer::SDLRenderer(Size viewport_base_size)
    : m_viewport_base_size(viewport_base_size)
{
    m_texture = nullptr;
    m_renderer = nullptr;
    m_text_texture = nullptr;
    m_font_biggest = nullptr;
    m_font_big = nullptr;
    m_font_normal = nullptr;

    m_fullscreen = false;
}

SDLRenderer::~SDLRenderer()
{
    if (m_renderer != nullptr)
        SDL_DestroyRenderer(m_renderer);
    if (m_texture != nullptr)
        SDL_DestroyTexture(m_texture);
    if (m_text_texture != nullptr)
        SDL_DestroyTexture(m_text_texture);
    if (m_font_biggest != nullptr)
        TTF_CloseFont(m_font_biggest);
    if (m_font_big != nullptr)
        TTF_CloseFont(m_font_big);
    if (m_font_normal != nullptr)
        TTF_CloseFont(m_font_normal);
}

void SDLRenderer::loadTexture(SDL_Window *window)
{
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // load surface
    if (m_renderer != nullptr)
        m_texture = IMG_LoadTexture(m_renderer, AppConfig::texture_path.c_str());
}

void SDLRenderer::loadFont()
{
    m_font_biggest = TTF_OpenFont(AppConfig::font_path.c_str(), 28);
    m_font_big = TTF_OpenFont(AppConfig::font_path.c_str(), 14);
    m_font_normal = TTF_OpenFont(AppConfig::font_path.c_str(), 10);
}

void SDLRenderer::clear()
{
    SDL_SetRenderDrawColor(m_renderer, 110, 110, 110, 255);
    SDL_RenderClear(m_renderer);
}

void SDLRenderer::flush()
{
    SDL_RenderPresent(m_renderer);
}

void SDLRenderer::drawObject(const Rect &texture_src, const Rect &window_dest)
{
    SDL_Rect t = {texture_src.x, texture_src.y, texture_src.w, texture_src.h};
    SDL_Rect w = {window_dest.x, window_dest.y, window_dest.w, window_dest.h};
    SDL_RenderCopy(m_renderer, m_texture, &t, &w);
}

void SDLRenderer::setViewportForWindowSize(Size window_size)
{
    int width = window_size.w;
    int height = window_size.h;

    if (width <= 0 || height <= 0)
        return;

    // Calculate the scale to maintain aspect ratio
    float scale = std::min(width / (float)m_viewport_base_size.w, height / (float)m_viewport_base_size.h);
    if (scale < 0.1)
        return;

    SDL_Rect viewport;
    viewport.x = (width / scale - m_viewport_base_size.w) / 2.0;
    viewport.y = (height / scale - m_viewport_base_size.h) / 2.0;
    if (viewport.x < 0)
        viewport.x = 0;
    if (viewport.y < 0)
        viewport.y = 0;
    viewport.w = m_viewport_base_size.w;
    viewport.h = m_viewport_base_size.h;

    SDL_RenderSetScale(m_renderer, scale, scale);
    SDL_RenderSetViewport(m_renderer, &viewport);
}

void SDLRenderer::drawText(const Point &start, std::string text, Color text_color, FontSize font_size)
{
    if (m_font_biggest == nullptr || m_font_big == nullptr || m_font_normal == nullptr)
        return;
    if (m_text_texture != nullptr)
        SDL_DestroyTexture(m_text_texture);

    SDL_Color sdl_text_color = {static_cast<Uint8>(text_color.r), static_cast<Uint8>(text_color.g), static_cast<Uint8>(text_color.b), static_cast<Uint8>(text_color.a)};
    SDL_Surface *text_surface = nullptr;

    switch (font_size)
    {
    case FontSize::NORMAL:
        text_surface = TTF_RenderText_Solid(m_font_normal, text.c_str(), sdl_text_color);
        break;
    case FontSize::BIG:
        text_surface = TTF_RenderText_Solid(m_font_big, text.c_str(), sdl_text_color);
        break;
    case FontSize::BIGGEST:
        text_surface = TTF_RenderText_Solid(m_font_biggest, text.c_str(), sdl_text_color);
        break;
    default:
        break;
    }

    if (text_surface == nullptr)
        return;

    m_text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    if (m_text_texture == nullptr)
        return;

    SDL_Rect window_dest;
    if (start.x < 0)
        window_dest.x = (m_viewport_base_size.w - text_surface->w) / 2;
    else
        window_dest.x = start.x;

    if (start.y < 0)
        window_dest.y = (m_viewport_base_size.h - text_surface->h) / 2;
    else
        window_dest.y = start.y;

    window_dest.w = text_surface->w;
    window_dest.h = text_surface->h;

    SDL_RenderCopy(m_renderer, m_text_texture, NULL, &window_dest);
}

void SDLRenderer::drawRect(const Rect &rect, Color rect_color, bool fill)
{
    SDL_Rect r = {rect.x, rect.y, rect.w, rect.h};
    SDL_SetRenderDrawColor(m_renderer, rect_color.r, rect_color.g, rect_color.b, rect_color.a);

    if (fill)
        SDL_RenderFillRect(m_renderer, &r);
    else
        SDL_RenderDrawRects(m_renderer, &r, 1);
}

void SDLRenderer::toggleFullscreen(SDL_Window *window)
{
    m_fullscreen = !m_fullscreen;

    if (m_fullscreen)
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    else
        SDL_SetWindowFullscreen(window, 0);
}
