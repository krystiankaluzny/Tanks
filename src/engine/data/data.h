#ifndef ENGINE_DATA_H
#define ENGINE_DATA_H

#include <stdint.h>

typedef uint32_t Uint32;
typedef int32_t Sint32;

struct Point
{
    int x;
    int y;
};


struct Rect
{
    int x, y;
    int w, h;

    bool isNotEmpty() const
    {
        return (w > 0 && h > 0);
    }
};

struct Size
{
    int w, h;
};

struct Color
{
    unsigned int r, g, b, a;
};

#endif //TANKS_DATA_H