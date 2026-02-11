#include "enemy.h"

Enemy::AliveState::AliveState(Enemy *enemy) : ContextState<Enemy>(enemy, enemy->m_enemy_state_machine)
{
    const SpriteData *sprite = &SpriteConfig::getInstance().getSpriteData(enemy->type);
    enemy->m_sprite = sprite;

    enemy->m_blocked = false;
    enemy->m_current_frame = 0;
    enemy->m_frame_display_time = 0;

    enemy->dest_rect = Rect{(int)enemy->pos_x, (int)enemy->pos_y, sprite->rect.w, sprite->rect.h};
}

void Enemy::AliveState::draw(Renderer &renderer)
{
    m_context->drawEnemy(renderer);
}

void Enemy::AliveState::update(const UpdateState &updateState)
{
    Enemy *enemy = m_context;
    const SpriteData *sprite = enemy->m_sprite;

    if (enemy->to_erase)
        return;

    enemy->updatePosition(updateState.delta_time);
    enemy->updateBullets(updateState.delta_time);
    enemy->updateEffects(updateState.delta_time);
    enemy->updateBehavior(updateState.delta_time);

    enemy->m_frame_display_time += updateState.delta_time;

    if (enemy->m_frame_display_time >= sprite->frame_duration && enemy->m_speed > 0)
    {
        enemy->m_frame_display_time = 0;
        enemy->m_current_frame++;
        if (enemy->m_current_frame >= sprite->frames_count && sprite->loop)
        {
            enemy->m_current_frame = 0;
        }
    }

    if (enemy->m_bonus)
        enemy->src_rect = sprite->rect.tiledOffset(enemy->m_tank_direction - 4, enemy->m_current_frame);
    else
        enemy->src_rect = sprite->rect.tiledOffset(enemy->m_tank_direction + (enemy->m_armor_count - 1) * 4, enemy->m_current_frame);

    enemy->m_blocked = false;
}