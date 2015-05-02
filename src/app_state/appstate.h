#ifndef APPSTATE_H
#define APPSTATE_H

#include <SDL2/SDL_events.h>
#include <string>

class AppState
{
public:
    virtual ~AppState() {}

    virtual bool finished() const = 0;
    virtual void draw() = 0;
    virtual void update(Uint32 dt) = 0;
    virtual void eventProcess(SDL_Event* ev) = 0;
    virtual AppState* nextState() = 0;

    static std::string intToString(int num)
    {
        if(num == 0) return "0";

        std::string buf;
        bool negative = false;
        if(num < 0)
        {
            negative = true;
            num = -num;
        }
        for(; num; num /= 10) buf = "0123456789abcdef"[num % 10] + buf;
        if(negative) buf = "-" + buf;
        return buf;
    }
};
#endif // APPSTATE_H
