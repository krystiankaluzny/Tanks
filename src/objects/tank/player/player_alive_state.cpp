#include "player.h"
#include "../../../appconfig.h"

Player::AliveState::AliveState(Player *player) : ContextState<Player>(player, player->m_player_state_machine)
{
}

void Player::AliveState::onInitialize()
{
    Player *player = m_context;

    const SpriteData *sprite = &SpriteConfig::getInstance().getSpriteData(player->type);
    player->m_sprite = sprite;

    player->m_blocked = false;
    player->m_current_frame = 0;
    player->m_frame_display_time = 0;

    player->dest_rect = Rect{(int)player->pos_x, (int)player->pos_y, sprite->rect.w, sprite->rect.h};
}

void Player::AliveState::draw(Renderer &renderer)
{
    m_context->drawPlayer(renderer);
}

void Player::AliveState::update(const UpdateState &updateState)
{
    Player *player = m_context;
    const SpriteData *sprite = player->m_sprite;

    if (player->to_erase)
        return;

    checkKeyStates(updateState);
    player->updatePosition(updateState.delta_time);
    player->updateBullets(updateState.delta_time);
    player->updateEffects(updateState.delta_time);

    player->m_fire_time += updateState.delta_time;
    player->m_frame_display_time += updateState.delta_time;

    if (player->m_frame_display_time >= sprite->frame_duration && player->m_speed > 0)
    {
        player->m_frame_display_time = 0;
        player->m_current_frame++;
        if (player->m_current_frame >= sprite->frames_count && sprite->loop)
        {
            player->m_current_frame = 0;
        }
    }

    player->src_rect = sprite->rect.tiledOffset(player->m_tank_direction, player->m_current_frame + 2 * player->m_armor_count);
    player->m_blocked = false;
}

void Player::AliveState::checkKeyStates(const UpdateState &updateState)
{
    Player *player = m_context;

    auto isPressed = [&](Direction d)
    {
        switch (d)
        {
        case D_UP:
            return player->m_key_state_up.pressed;
        case D_DOWN:
            return player->m_key_state_down.pressed;
        case D_LEFT:
            return player->m_key_state_left.pressed;
        case D_RIGHT:
            return player->m_key_state_right.pressed;
        default:
            return false;
        }
    };

    if (isPressed(player->m_last_pressed_direction))
    {
        player->setDirection(player->m_last_pressed_direction);
        player->m_speed = player->m_max_speed;
    }
    else if (player->m_key_state_up.pressed)
    {
        player->setDirection(D_UP);
        player->m_speed = player->m_max_speed;
    }
    else if (player->m_key_state_down.pressed)
    {
        player->setDirection(D_DOWN);
        player->m_speed = player->m_max_speed;
    }
    else if (player->m_key_state_left.pressed)
    {
        player->setDirection(D_LEFT);
        player->m_speed = player->m_max_speed;
    }
    else if (player->m_key_state_right.pressed)
    {
        player->setDirection(D_RIGHT);
        player->m_speed = player->m_max_speed;
    }
    else
    {
        if (!player->isSlipping())
            player->m_speed = 0.0;
    }

    if (player->m_key_state_fire.pressed && player->m_fire_time > AppConfig::player_reload_time)
    {
        player->fire();
        player->m_fire_time = 0;
    }
}