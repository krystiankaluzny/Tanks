#ifndef SOUNDCONFIG_H
#define SOUNDCONFIG_H

#include "engine/sound_manager.h"

namespace SoundConfig
{
    const Sound BONUS_APPEAR = {"sounds/bonus_appear.wav", 100.0, false};
    const Sound BONUS_OBTAIN = {"sounds/bonus_obtain.wav", 100.0, false};
    const Sound BULLET_HIT_BRICK = {"sounds/bullet_hit_brick.wav", 100.0, false};
    const Sound BULLET_HIT_ENEMY = {"sounds/bullet_hit_enemy.wav", 100.0, false};
    const Sound BULLET_HIT_WALL = {"sounds/bullet_hit_wall.wav", 100.0, false};
    const Sound EAGLE_DESTROYED = {"sounds/eagle_destroyed.wav", 100.0, false};
    const Sound ENEMY_DESTROYED = {"sounds/enemy_destroyed.wav", 100.0, false};
    const Sound GAME_OVER = {"sounds/game_over.wav", 100.0, false};
    const Sound high_score = {"sounds/high_score.wav", 100.0, false};
    const Sound PAUSE = {"sounds/pause.wav", 100.0, false};
    const Sound PLAYER_DESTROYED = {"sounds/player_destroyed.wav", 100.0, false};
    const Sound PLAYER_FIRE = {"sounds/player_fire.wav", 100.0, false};
    const Sound PLAYER_IDLE = {"sounds/player_idle.wav", 100.0, false};
    const Sound PLAYER_LEVEL_UP = {"sounds/player_level_up.wav", 100.0, false};
    const Sound PLAYER_MOVING = {"sounds/player_moving.wav", 100.0, false};
    const Sound PLAYER_SLIDING = {"sounds/player_sliding.wav", 100.0, false};
    const Sound SCORE_BONUS_POINTS = {"sounds/score_bonus_points.wav", 100.0, false};
    const Sound SCORE_COUNT = {"sounds/score_count.wav", 100.0, false};
    const Sound STAGE_START_UP = {"sounds/stage_start_up.wav", 100.0, false};
}

#endif