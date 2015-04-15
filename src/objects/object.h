#ifndef OBJECT_H
#define OBJECT_H

#include "../engine/engine.h"

class Object
{
public:
    Object();
    Object(double x, double y, SpriteType type);
    Object(double x, double y, const SpriteData* sprite);
    virtual void draw();
    virtual void update(Uint32 dt);

    double pos_x;
    double pos_y;
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    SDL_Rect collision_rect;
    SpriteType type;
    bool to_erase;

protected:
    SDL_Rect moveRect(const SDL_Rect &rect, int x, int y); //zwraca prostokątk przesunięty o (x*w; y*h) pikseli względem rect

    const SpriteData* m_sprite; //nie należy tego tu usuwać
    int m_current_frame;
    Uint32 m_frame_display_time;
};

SDL_Rect intersectRect(SDL_Rect* rect1, SDL_Rect* rect2);

#endif // OBJECT_H
