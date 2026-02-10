#include "enemy.h"
#include "../../appconfig.h"

Enemy::DestroyedState::DestroyedState(Enemy *enemy) : ContextState<Enemy>(enemy, enemy->m_enemy_state_machine)
{
    const SpriteData *sprite = &SpriteConfig::getInstance().getSpriteData(ST_DESTROY_TANK);
    enemy->m_sprite = sprite;

    enemy->m_speed = 0.0;
    enemy->m_blocked = true;
    enemy->m_current_frame = 0;
    enemy->m_frame_display_time = 0;

    enemy->dest_rect.x = enemy->pos_x + (enemy->dest_rect.w - sprite->rect.w) / 2;
    enemy->dest_rect.y = enemy->pos_y + (enemy->dest_rect.h - sprite->rect.h) / 2;
    enemy->dest_rect.h = sprite->rect.h;
    enemy->dest_rect.w = sprite->rect.w;

    enemy->collision_rect = Rect{0, 0, 0, 0};
}

void Enemy::DestroyedState::draw(Renderer &renderer)
{
    m_context->drawEnemy(renderer);
}

void Enemy::DestroyedState::update(const UpdateState &updateState)
{
    Enemy *enemy = m_context;
    const SpriteData *sprite = enemy->m_sprite;

    if (enemy->to_erase)
        return;

    if (enemy->m_lives_count == 0 && enemy->bullets.size() == 0)
    {
        enemy->to_erase = true;
        return;
    }

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

            if (enemy->m_lives_count > 0)
            {
                enemy->m_lives_count--;
            }
        }
    }

    enemy->src_rect = sprite->rect.tiledOffset(0, enemy->m_current_frame);
}