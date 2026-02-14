#ifndef SOUNDCONFIG_H
#define SOUNDCONFIG_H

#include "engine/sound_manager.h"

namespace SoundConfig
{
    const Sound STAGE_START_UP{"sounds/holoflash_stage_start_up.wav", 100.0, 1.0, false, true};
    const Sound PAUSE{"sounds/holoflash_pause.wav", 100.0, 1.0, false};
    const Sound GAME_OVER{"sounds/holoflash_game_over.wav", 100.0, 1.0, false};

    const Sound BONUS_APPEARED{"sounds/holoflash_bonus_appeared.wav", 100.0, 0.9, true};
    const Sound BONUS_OBTAINED{"sounds/holoflash_bonus_obtained.wav", 100.0, 0.9, true};

    const Sound BULLET_HIT_BRICK{"sounds/holoflash_bullet_hit_brick.wav", 60.0, 0.7, true};
    const Sound BULLET_HIT_MAP_BOUNDARIES{"sounds/holoflash_bullet_hit_map_boundaries.wav", 60.0, 1.0, true};
    const Sound BULLET_HIT_STONE{"sounds/holoflash_bullet_hit_stone.wav", 60.0, 1.0, true};

    const Sound EAGLE_DESTROYED{"sounds/holoflash_eagle_destroyed.wav", 100.0, 1.0, true};

    const Sound ENEMY_DESTROYED{"sounds/holoflash_enemy_destroyed.wav", 100.0, 0.7, true};
    const Sound ENEMY_HIT{"sounds/holoflash_enemy_hit.wav", 70.0, 1.0, true};

    // const Sound HIGHSCORE_BEATEN {"sounds/holoflash_highscore_beaten.wav", 100.0, true};

    const Sound MENU_ITEM_SELECTED{"sounds/holoflash_menu_item_selected.wav", 70.0, 1.0, true};

    const Sound PLAYER_DESTROYED{"sounds/holoflash_player_destroyed.wav", 60.0, 1.0, true};
    const Sound PLAYER_FIRED{"sounds/holoflash_player_fired.wav", 60.0, 1.0, true};
    const Sound PLAYER_HIT{"sounds/holoflash_player_hit.wav", 100.0, 1.0, true};
    const Sound PLAYER_IDLE{"sounds/holoflash_player_idle.wav", 40.0, 1.0, false};
    const Sound PLAYER_LIFE_UP{"sounds/holoflash_player_life_up.wav", 100.0, 1.0, true};
    const Sound PLAYER_MOVING{"sounds/holoflash_player_moving.wav", 40.0, 1.0, false};
    // const Sound PLAYER_SLIDING{"sounds/holoflash_player_sliding.wav", 100.0, true};

    // const Sound SCORE_BONUS_POINTS_RECEIVED {"sounds/holoflash_score_bonus_points_received.wav", 100.0, 1.0, true};

    const Sound SCORE_POINT_COUNTED{"sounds/holoflash_score_point_counted.wav", 80.0, 1.0, true};
}

#endif