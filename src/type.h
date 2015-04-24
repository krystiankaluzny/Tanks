#ifndef TYPE_H
#define TYPE_H

enum SpriteType
{
    //A - czołgi podstawowe
    //B - szybko jężdzące
    //C - szybko strzelające
    //D -  ? opancerzone ?
    ST_TANK_A,
    ST_TANK_B,
    ST_TANK_C,
    ST_TANK_D,

    ST_PLAYER_1,
    ST_PLAYER_2,

    ST_BRICK_WALL,
    ST_STONE_WALL,
    ST_WATER,
    ST_BUSH, //krzaki
    ST_ICE,

    ST_BONUS_GRANATE,
    ST_BONUS_HELMET,
    ST_BONUS_CLOCK,
    ST_BONUS_SHOVEL, //łopata
    ST_BONUS_TANK,
    ST_BONUS_STAR,
    ST_BONUS_GUN,
    ST_BONUS_BOAT,

    ST_SHIELD,
    ST_CREATE,
    ST_DESTROY_TANK,
    ST_DESTROY_BULLET,
    ST_BOAT_P1,
    ST_BOAT_P2,

    ST_EAGLE,
    ST_DESTROY_EAGLE,
    ST_FLAG,

    ST_BULLET,

    ST_NONE
    //TODO
    //pozostali przeciwnicy
    //pozostałe życia
    //numer rundy
};

enum TankStateFlag
{
    TSF_SHIELD = 1, //po wzięciu hełmu
    TSF_FROZEN = 2, //po wzięciu zegara przez przeciwnika
    TSF_DESTROYED = 4, //po wzięciu bomby prez przeciwnika lub trafieniu kulą
    TSF_BRIDGE = 8, //po wzięciu łódki, pozwala przechodzić przez wodę
    TSF_BONUS = 16, //po trafieniu tego czołgu na mapie pojawi się bonus
    TSF_ON_ICE = 32,  //jeżeli czołg jest na lodzie to się ślizga
    TSF_CREATE = 64, //tworzenie czołgu
    TSF_LIFE = 128
};

enum Direction
{
    D_UP = 0,
    D_RIGHT = 1,
    D_DOWN = 2,
    D_LEFT = 3
};

/*
enum LevelTile
{
    LT_EMPTY,
    LT_BRICK,
    LT_STONE,
    LT_WATER,
    LT_BUSH,
    LT_ROAD
};
*/
#endif // TYPE_H
