#include "spriteconfig.h"

SpriteConfig::SpriteConfig()
{
    insert(ST_TANK_A, 128, 0, 32, 32, 2, 100, true);
    insert(ST_TANK_B, 128, 64, 32, 32, 2, 100, true);
    insert(ST_TANK_C, 128, 128, 32, 32, 2, 100, true);
    insert(ST_TANK_D, 128, 192, 32, 32, 2, 100, true);

    insert(ST_PLAYER_1, 640, 0, 32, 32, 2, 50, true);
    insert(ST_PLAYER_2, 768, 0, 32, 32, 2, 50, true);

    insert(ST_BRICK_WALL, 928, 0, 16, 16, 1, 200, false);
    insert(ST_STONE_WALL, 928, 144, 16, 16, 1, 200, false);
    insert(ST_WATER, 928, 160, 16, 16, 2, 350, true);
    insert(ST_BUSH, 928, 192, 16, 16, 1, 200, false);
    insert(ST_ICE, 928, 208, 16, 16, 1, 200, false);

    insert(ST_BONUS_GRENADE, 896, 0, 32, 32, 1, 200, false);
    insert(ST_BONUS_HELMET, 896, 32, 32, 32, 1, 200, false);
    insert(ST_BONUS_CLOCK, 896, 64, 32, 32, 1, 200, false);
    insert(ST_BONUS_SHOVEL, 896, 96, 32, 32, 1, 200, false);
    insert(ST_BONUS_TANK, 896, 128, 32, 32, 1, 200, false);
    insert(ST_BONUS_STAR, 896, 160, 32, 32, 1, 200, false);
    insert(ST_BONUS_GUN, 896, 192, 32, 32, 1, 200, false);
    insert(ST_BONUS_BOAT, 896, 224, 32, 32, 1, 200, false);

    insert(ST_SHIELD, 976, 0, 32, 32, 2, 45, true);
    insert(ST_CREATE, 1008, 0, 32, 32, 10, 100, false);
    insert(ST_DESTROY_TANK, 1040, 0, 64, 64, 7, 70, false);
    insert(ST_DESTROY_BULLET, 1108, 0, 32, 32, 5, 40, false);
    insert(ST_BOAT_P1, 944, 96, 32, 32, 1, 200, false);
    insert(ST_BOAT_P2, 976, 96, 32, 32, 1, 200, false);

    insert(ST_EAGLE, 944, 0, 32, 32, 1, 200, false);
    insert(ST_DESTROY_EAGLE, 1040, 0, 64, 64, 7, 100, false);
    insert(ST_FLAG, 944, 32, 32, 32, 1, 200, false);

    insert(ST_BULLET, 944, 128, 8, 8, 1, 200, false);

    insert(ST_LEFT_ENEMY, 944, 144, 16, 16, 1, 200, false);
    insert(ST_STAGE_STATUS, 976, 64, 32, 32, 1, 200, false);

    insert(ST_TANKS_LOGO, 0, 260, 406, 72, 1, 200, false);
}

const SpriteData* SpriteConfig::getSpriteData(SpriteType st) const
{
    return &m_configs.at(st);
}

void SpriteConfig::insert(SpriteType st, int x, int y, int w, int h, int fc, int fd, bool l)
{
    m_configs[st] = SpriteData(x, y, w, h, fc, fd, l);
}
