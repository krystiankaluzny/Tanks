#include "enemy.h"

Enemy::CreatingState::CreatingState(Enemy *enemy) : ContextState<Enemy>(enemy, enemy->m_enemy_state_machine)
{
    const SpriteData *sprite = &SpriteConfig::getInstance().getSpriteData(ST_CREATE);
    enemy->m_sprite = sprite;

    enemy->m_speed = 0.0;
    enemy->m_blocked = false;
    enemy->m_current_frame = 0;
    enemy->m_frame_display_time = 0;

    enemy->dest_rect = Rect{(int)enemy->pos_x, (int)enemy->pos_y, sprite->rect.w, sprite->rect.h};
    enemy->collision_rect = Rect{0, 0, 0, 0};

    enemy->setDirection(D_DOWN);

    for (auto b : enemy->bullets)
        b->to_erase = true;
}

void Enemy::CreatingState::draw(Renderer &renderer)
{
    m_context->drawEnemy(renderer);
}

void Enemy::CreatingState::update(const UpdateState &updateState)
{
    Enemy *enemy = m_context;
    const SpriteData *sprite = enemy->m_sprite;

    enemy->updateBullets(updateState.delta_time);
    enemy->updateEffects(updateState.delta_time);

    enemy->m_frame_display_time += updateState.delta_time;

    if (enemy->m_frame_display_time >= sprite->frame_duration)
    {
        enemy->m_frame_display_time = 0;
        enemy->m_current_frame++;
        if (enemy->m_current_frame >= sprite->frames_count)
        {
            enemy->m_current_frame = 0;
            transiteTo(new Enemy::AliveState(enemy));
            return;
        }
    }

    enemy->src_rect = sprite->rect.tiledOffset(0, enemy->m_current_frame);
    enemy->m_blocked = false;
}