#include "player.h"
#include "../../../appconfig.h"

Player::CreatingState::CreatingState(Player *player) : ContextState<Player>(player, player->m_player_state_machine)
{
}

void Player::CreatingState::onInitialize()
{
    Player *player = m_context;

    if (player->type == ST_PLAYER_1)
    {
        player->pos_x = AppConfig::player_starting_point.at(0).x;
        player->pos_y = AppConfig::player_starting_point.at(0).y;
    }
    else
    {
        player->pos_x = AppConfig::player_starting_point.at(1).x;
        player->pos_y = AppConfig::player_starting_point.at(1).y;
    }

    const SpriteData *sprite = &SpriteConfig::getInstance().getSpriteData(ST_CREATE);
    player->m_sprite = sprite;

    player->m_speed = 0.0;
    player->m_blocked = false;
    player->m_current_frame = 0;
    player->m_frame_display_time = 0;

    player->src_rect = sprite->rect.tiledOffset(0, player->m_current_frame);
    player->dest_rect = Rect{(int)player->pos_x, (int)player->pos_y, sprite->rect.w, sprite->rect.h};
    player->collision_rect = Rect{0, 0, 0, 0};

    player->setDirection(D_UP);
    player->resetKeyStates();
    player->activateShieldEffect();

    for (auto b : player->bullets)
        b->to_erase = true;
}

void Player::CreatingState::draw(Renderer &renderer)
{
    m_context->drawPlayer(renderer);
}

void Player::CreatingState::update(const UpdateState &updateState)
{
    Player *player = m_context;
    const SpriteData *sprite = player->m_sprite;

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
            transiteTo(new Player::AliveState(player));
            return;
        }
    }

    player->src_rect = sprite->rect.tiledOffset(0, player->m_current_frame);
    player->m_blocked = false;
}