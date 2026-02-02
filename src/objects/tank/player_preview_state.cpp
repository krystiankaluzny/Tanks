#include "player.h"
#include "../../appconfig.h"

Player::PreviewState::PreviewState(Player *player) : ContextState<Player>(player, player->m_player_state_machine)
{
    const SpriteData *sprite = &SpriteConfig::getInstance().getSpriteData(player->type);
    player->m_sprite = sprite;

    player->m_speed = 0.0;
    player->m_blocked = false;
    player->m_current_frame = 0;
    player->m_frame_display_time = 0;

    player->dest_rect = Rect{(int)player->pos_x, (int)player->pos_y, sprite->rect.w, sprite->rect.h};
    player->collision_rect = Rect{0, 0, 0, 0};

    player->deactivateAllEffects();
    player->setDirection(D_RIGHT);
    player->resetKeyStates();

    for (auto b : player->bullets)
        b->to_erase = true;
}

void Player::PreviewState::draw(Renderer &renderer)
{
    m_context->drawPlayer(renderer);
}

void Player::PreviewState::update(const UpdateState &updateState)
{
    Player *player = m_context;
    const SpriteData *sprite = player->m_sprite;

    player->updatePosition(updateState.delta_time);
    player->updateBullets(updateState.delta_time);
    player->updateEffects(updateState.delta_time);

    player->m_frame_display_time += updateState.delta_time;

    //double animation speed
    if (player->m_frame_display_time >= sprite->frame_duration / 2)
    {
        player->m_frame_display_time = 0;
        player->m_current_frame++;
        if (player->m_current_frame >= sprite->frames_count && sprite->loop)
        {
            player->m_current_frame = 0;
            return;
        }
    }

    player->src_rect = sprite->rect.tiledOffset(player->m_tank_direction, player->m_current_frame);
    player->m_blocked = false;
}