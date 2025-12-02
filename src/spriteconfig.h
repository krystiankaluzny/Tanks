#ifndef SPRITECONFIG_H
#define SPRITECONFIG_H

#include "engine/data/data.h"
#include <map>

enum SpriteType
{
    ST_TANK_A,
    ST_TANK_B,
    ST_TANK_C,
    ST_TANK_D,

    ST_PLAYER_1,
    ST_PLAYER_2,

    ST_BRICK_WALL,
    ST_STONE_WALL,
    ST_WATER,
    ST_BUSH,
    ST_ICE,

    ST_BONUS_GRENADE,
    ST_BONUS_HELMET,
    ST_BONUS_CLOCK,
    ST_BONUS_SHOVEL,
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

    ST_LEFT_ENEMY,
    ST_STAGE_STATUS,

    ST_TANKS_LOGO,

    ST_NONE
};

/**
 * @brief
 * Structure storing information about a given object animation type.
 */
struct SpriteData
{
    /**
     * Position and dimensions of the first animation frame
     */
    Rect rect;
    /**
     * Number of frames in the animation.
     */
    int frames_count;
    /**
     * Display time of one frame in milliseconds.
     */
    unsigned frame_duration;
    /**
     * Variable determining whether the animation is looped.
     */
    bool loop;
};

/**
 * @brief
 * Class that stores information about all animation types in the game.
 */
class SpriteConfig
{
public:
    static SpriteConfig &getInstance()
    {
        static SpriteConfig instance;
        return instance;
    }

    /**
     * Get the selected animation type.
     * @param sp - requested animation type
     * @return animation of the given type
     */
    const SpriteData &getSpriteData(SpriteType sp) const;

private:
    SpriteConfig();
    ~SpriteConfig() = default;
    SpriteConfig(const SpriteConfig &) = delete;
    SpriteConfig &operator=(const SpriteConfig &) = delete;

    /**
     * Container storing all animation types.
     */
    std::map<SpriteType, SpriteData> m_configs;
    /**
     * Function used when adding a new animation type.
     * @param st - animation type
     * @param rect - position and dimensions of the first animation frame in the texture
     * @param frames_count - number of frames in the animation
     * @param frame_duration - display time of one frame
     * @param loop - animation looping flag
     */
    void insert(SpriteType st, Rect rect, int frames_count, unsigned frame_duration, bool loop);
};

#endif // SPRITECONFIG_H
