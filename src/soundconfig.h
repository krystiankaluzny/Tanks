#ifndef SOUNDCONFIG_H
#define SOUNDCONFIG_H

#include "engine/sound_manager.h"

namespace SoundConfig
{
    const Sound STAGE_START_UP{"sounds/stage_start_up.wav", 100.0, 1.0, false, true};
    const Sound PAUSE{"sounds/pause.wav", 100.0, 1.0, false};
    const Sound GAME_OVER{"sounds/game_over.wav", 100.0, 1.0, false};

    const Sound BONUS_APPEARED{"sounds/bonus_appeared.wav", 100.0, 0.9, true};
    const Sound BONUS_OBTAINED{"sounds/bonus_obtained.wav", 100.0, 0.9, true};

    const Sound BULLET_HIT_BRICK{"sounds/bullet_hit_brick.wav", 40.0, 0.7, true};
    const Sound BULLET_HIT_MAP_BOUNDARIES{"sounds/bullet_hit_map_boundaries.wav", 40.0, 1.0, true};
    const Sound BULLET_HIT_STONE{"sounds/bullet_hit_stone.wav", 40.0, 1.0, true};

    const Sound EAGLE_DESTROYED{"sounds/eagle_destroyed.wav", 70.0, 1.0, true};

    const Sound ENEMY_DESTROYED{"sounds/enemy_destroyed.wav", 100.0, 0.7, true};
    const Sound ENEMY_HIT{"sounds/enemy_hit.wav", 70.0, 1.0, true};

    // const Sound HIGHSCORE_BEATEN {"sounds/highscore_beaten.wav", 100.0, true};

    const Sound MENU_ITEM_SELECTED{"sounds/menu_item_selected.wav", 70.0, 1.0, true};

    const Sound PLAYER_DESTROYED{"sounds/player_destroyed.wav", 60.0, 1.0, true};
    const Sound PLAYER_FIRED{"sounds/player_fired.wav", 60.0, 1.0, true};
    const Sound PLAYER_HIT{"sounds/player_hit.wav", 100.0, 1.0, true};
    const Sound PLAYER_IDLE{"sounds/player_idle.wav", 50.0, 1.0, false};
    const Sound PLAYER_LIFE_UP{"sounds/player_life_up.wav", 100.0, 1.0, true};
    const Sound PLAYER_MOVING{"sounds/player_moving.wav", 50.0, 1.0, false};
    // const Sound PLAYER_SLIDING{"sounds/player_sliding.wav", 100.0, true};

    // const Sound SCORE_BONUS_POINTS_RECEIVED {"sounds/score_bonus_points_received.wav", 100.0, 1.0, true};

    const Sound SCORE_POINT_COUNTED{"sounds/score_point_counted.wav", 80.0, 1.0, true};
}

#endif