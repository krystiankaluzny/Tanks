#ifndef OBJECT_H
#define OBJECT_H

#include "../engine/engine.h"
#include "../engine/data/data.h"
#include "../spriteconfig.h"


class Object
{
public:
    Object(double x, double y, SpriteType type);

    virtual ~Object();


    virtual void draw(Renderer &renderer);
    virtual void update(Uint32 dt);


    bool to_erase;
    Rect collision_rect;
    Rect dest_rect;
    Rect src_rect;
    SpriteType type;
    double pos_x;
    double pos_y;

protected:
    const SpriteData* m_sprite;
    Uint32 m_frame_display_time;
    int m_current_frame;
};

#endif // OBJECT_H
