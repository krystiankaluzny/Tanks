#ifndef SPRITECONFIG_H
#define SPRITECONFIG_H

#include <map>
#include "../type.h"
#include <SDL2/SDL.h>

struct SpriteData
{
    SpriteData() { rect.x = 0; rect.y = 0; rect.w = 0; rect.h = 0; frames_count = 0; frame_duration = 0; loop = false;}
    SpriteData(int x, int y, int w, int h, int fc, int fd, bool l)
        { rect.x = x; rect.y = y; rect.w = w; rect.h = h; frames_count = fc; frame_duration = fd; loop = l;}

    SDL_Rect rect;
    int frames_count;

    unsigned frame_duration; //czas wyświetlania jednej klatki ms
    bool loop;
};

class SpriteConfig
{
public:
    SpriteConfig();
    const SpriteData* getSpriteData(SpriteType sp) const;
private:
    std::map<SpriteType, SpriteData> m_configs;
    void insert(SpriteType st , int x, int y, int w, int h, int fc, int fd, bool l);
};

#endif // SPRITECONFIG_H
