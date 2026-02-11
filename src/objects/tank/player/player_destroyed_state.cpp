#include "player.h"

Player::DestroyedState::DestroyedState(Player *player) : ContextState<Player>(player, player->m_player_state_machine)
{
    const SpriteData *sprite = &SpriteConfig::getInstance().getSpriteData(ST_DESTROY_TANK);
    player->m_sprite = sprite;

    player->m_speed = 0.0;
    player->m_blocked = true;
    player->m_current_frame = 0;
    player->m_frame_display_time = 0;

    player->dest_rect.x = player->pos_x + (player->dest_rect.w - sprite->rect.w) / 2;
    player->dest_rect.y = player->pos_y + (player->dest_rect.h - sprite->rect.h) / 2;
    player->dest_rect.h = sprite->rect.h;
    player->dest_rect.w = sprite->rect.w;

    player->collision_rect = Rect{0, 0, 0, 0};
}

void Player::DestroyedState::draw(Renderer &renderer)
{
    m_context->drawPlayer(renderer);
}

void Player::DestroyedState::update(const UpdateState &updateState)
{
    Player *player = m_context;
    const SpriteData *sprite = player->m_sprite;

    if (player->to_erase)
        return;

    if (player->m_lives_count == 0 && player->bullets.size() == 0)
    {
        player->to_erase = true;
        return;
    }

    player->updateBullets(updateState.delta_time);
    player->updateEffects(updateState.delta_time);

    player->m_frame_display_time += updateState.delta_time;

    if (player->m_frame_display_time >= sprite->frame_duration)
    {
        player->m_frame_display_time = 0;
        player->m_current_frame++;
        if (player->m_current_frame >= sprite->frames_count)
        {
            player->m_current_frame = 0;

            // After DESTROYED decreasing lives
            if (player->m_lives_count == 1)
            {
                player->m_lives_count = 0;
            }
            else if (player->m_lives_count > 1)
            {
                player->m_lives_count--;
                player->respawn();
            }
        }
    }

    player->src_rect = sprite->rect.tiledOffset(0, player->m_current_frame);
}