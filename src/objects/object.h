#ifndef OBJECT_H
#define OBJECT_H

#include "../engine/engine.h"


class Object
{
public:
    Object();
    Object(double x, double y, SpriteType type);
    Object(double x, double y, const SpriteData* sprite);
    virtual ~Object();
    virtual void draw();
    virtual void update(Uint32 dt);

    bool to_erase;
    SDL_Rect collision_rect;
    SDL_Rect dest_rect;
    SDL_Rect src_rect;
    SpriteType type;
    double pos_x;
    double pos_y;

protected:
    SDL_Rect moveRect(const SDL_Rect &rect, int x, int y); //zwraca prostokątk przesunięty o (x*w; y*h) pikseli względem rect

    const SpriteData* m_sprite; //nie należy tego tu usuwać
    Uint32 m_frame_display_time;
    int m_current_frame;
};

SDL_Rect intersectRect(SDL_Rect* rect1, SDL_Rect* rect2);

#endif // OBJECT_H
