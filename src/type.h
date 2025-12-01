#ifndef TYPE_H
#define TYPE_H



enum TankStateFlag
{
    TSF_SHIELD = 1 << 1, //po wzięciu hełmu
    TSF_FROZEN = 1 << 2, //po wzięciu zegara przez przeciwnika
    TSF_DESTROYED = 1 << 3, //po wzięciu bomby prez przeciwnika lub trafieniu kulą
    TSF_BOAT = 1 << 4, //po wzięciu łódki, pozwala przechodzić przez wodę
    TSF_BONUS = 1 << 5, //po trafieniu tego czołgu na mapie pojawi się bonus
    TSF_ON_ICE = 1 << 6,  //jeżeli czołg jest na lodzie to się ślizga
    TSF_CREATE = 1 << 7, //tworzenie czołgu
    TSF_LIFE = 1 << 8,
    TSF_MENU = 1 << 9 //dwukrotne przyspieszenie animacji
};

enum Direction
{
    D_UP = 0,
    D_RIGHT = 1,
    D_DOWN = 2,
    D_LEFT = 3
};

#endif // TYPE_H
