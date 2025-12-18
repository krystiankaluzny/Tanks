#ifndef ENGINE_DATA_H
#define ENGINE_DATA_H

#include <stdint.h>
#include <algorithm>

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

    Rect intersection(Rect &other) const
    {
        int intersect_x = std::max(x, other.x);
        int intersect_y = std::max(y, other.y);

        return Rect{
            intersect_x,
            intersect_y,
            std::min(x + w, other.x + other.w) - intersect_x,
            std::min(y + h, other.y + other.h) - intersect_y};
    }

    Rect tiledOffset(int x_tile_offset, int y_tile_offset) const
    {
        return Rect{
            x + x_tile_offset * w,
            y + y_tile_offset * h,
            w, h};
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

#endif // TANKS_DATA_H