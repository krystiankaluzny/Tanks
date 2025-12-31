#ifndef SOUNDCONFIG_H
#define SOUNDCONFIG_H

#include "engine/sound_manager.h"

namespace SoundConfig
{
    const Sound STAGE_START_UP { "sounds/stage_start_up.wav", 100.0, 1.0, false, true};
    const Sound PAUSE {"sounds/pause.wav", 40.0, 1.0, false};
    const Sound GAME_OVER {"sounds/game_over.wav", 50.0, 1.0, false};

    const Sound MENU_CHANGE_POS {"sounds/other/SFX 11.mp3", 100.0, 1.0, true};
    // const Sound PLAYER_IDLE {"sounds/player_idle.wav", 100.0, false};
    const Sound PLAYER_IDLE {"sounds/other_2/4_tank_standing.mp3", 100.0, 1.0, false};
    // const Sound PLAYER_MOVING {"sounds/player_moving.wav", 100.0, false};
    const Sound PLAYER_MOVING {"sounds/other_2/3_tank_moving.mp3", 100.0, 1.0, false};
    const Sound PLAYER_DESTROYED {"sounds/player_destroyed.wav", 100.0, 1.0, true};
    const Sound PLAYER_HIT {"sounds/bullet_hit_enemy.wav", 100.0, 1.0, true};
    const Sound PLAYER_FIRE {"sounds/player_fire.wav", 100.0, 1.0, true};
    const Sound PLAYER_LEVEL_UP {"sounds/player_level_up.wav", 100.0, 1.0, true};
    // const Sound PLAYER_SLIDING {"sounds/player_sliding.wav", 100.0, true};

    const Sound BONUS_APPEAR {"sounds/bonus_appear.wav", 100.0, 0.9, true};
    const Sound BONUS_OBTAIN {"sounds/bonus_obtain.wav", 100.0, 0.9, true};

    const Sound BULLET_HIT_BRICK {"sounds/bullet_hit_brick.wav", 100.0, 1.0, true};
    const Sound BULLET_HIT_WALL {"sounds/bullet_hit_wall.wav", 100.0, 1.0, true};

    const Sound EAGLE_DESTROYED {"sounds/eagle_destroyed.wav", 100.0, 1.0, true};
    const Sound ENEMY_DESTROYED {"sounds/enemy_destroyed.wav", 100.0, 0.7, true};
    const Sound ENEMY_HIT {"sounds/bullet_hit_enemy.wav", 100.0, 1.0, true};

    // const Sound HIGH_SCORE {"sounds/high_score.wav", 100.0, true};

    // const Sound SCORE_BONUS_POINTS {"sounds/score_bonus_points.wav", 100.0, true};
    // const Sound SCORE_COUNT {"sounds/score_count.wav", 100.0, true};
}

#endif