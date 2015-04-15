#ifndef PLAYER_H
#define PLAYER_H

#include "tank.h"

class Player : public Tank
{
public:
    Player();
    Player(double x, double y, SpriteType type);

    void update(Uint32 dt);
    void respawn();

};

#endif // PLAYER_H
