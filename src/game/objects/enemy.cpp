#include "enemy.h"
#include "../../appconfig.h"
#include "../../event/event.h"
#include "../../appthread.h"

#include <stdlib.h>
#include <ctime>
#include <iostream>

Enemy::Enemy()
    : Tank(AppConfig::enemy_starting_point.at(0).x, AppConfig::enemy_starting_point.at(0).y, ST_TANK_A)
{
    direction = D_DOWN;
    m_direction_time = 0;
    m_keep_direction_time = 100;

    m_speed_time = 0;
    m_try_to_go_time = 100;

    m_fire_time = 0;
    m_reload_time = 100;
    lives_count = 1;

    m_bullet_max_size = 1;

    m_frozen_time = 0;

    if(type == ST_TANK_B)
        default_speed = AppConfig::tank_default_speed * 1.3;
    else
        default_speed = AppConfig::tank_default_speed;

    target_position = {-1, -1};

    respawn();
}

Enemy::Enemy(double x, double y, SpriteType type)
    : Tank(x, y, type)
{
    direction = D_DOWN;
    m_direction_time = 0;
    m_keep_direction_time = 100;

    m_speed_time = 0;
    m_try_to_go_time = 100;

    m_fire_time = 0;
    m_reload_time = 100;
    lives_count = 1;

    m_bullet_max_size = 1;

    m_frozen_time = 0;

    if(type == ST_TANK_B)
        default_speed = AppConfig::tank_default_speed * 1.3;
    else
        default_speed = AppConfig::tank_default_speed;

    target_position = {-1, -1};

    respawn();
}

void Enemy::draw()
{
    if(to_erase) return;
    if(AppConfig::show_enemy_target)
    {
        SDL_Color c;
        if(type == ST_TANK_A) c = {250, 0, 0, 250};
        if(type == ST_TANK_B) c = {0, 0, 250, 255};
        if(type == ST_TANK_C) c = {0, 255, 0, 250};
        if(type == ST_TANK_D) c = {250, 0, 255, 250};
        SDL_Rect r = {min(target_position.x, dest_rect.x + dest_rect.w / 2), dest_rect.y + dest_rect.h / 2, abs(target_position.x - (dest_rect.x + dest_rect.w / 2)), 1};
        Engine::getEngine().getRenderer()->drawRect(&r, c,  true);
        r = {target_position.x, min(target_position.y, dest_rect.y + dest_rect.h / 2), 1, abs(target_position.y - (dest_rect.y + dest_rect.h / 2))};
        Engine::getEngine().getRenderer()->drawRect(&r, c, true);
    }
    Tank::draw();
}

void Enemy::update(Uint32 dt)
{
    if(to_erase) return;
    Tank::update(dt);

    if(testFlag(TSF_LIFE))
    {
        if(testFlag(TSF_BONUS))
            src_rect = moveRect(m_sprite->rect, (testFlag(TSF_ON_ICE) ? new_direction : direction) - 4, m_current_frame);
        else
            src_rect = moveRect(m_sprite->rect, (testFlag(TSF_ON_ICE) ? new_direction : direction) + (lives_count -1) * 4, m_current_frame);
    }
    else
        src_rect = moveRect(m_sprite->rect, 0, m_current_frame);

    if(testFlag(TSF_FROZEN)) return;

    NetworkState state = NetworkState::NONE;
    if(parent != nullptr)
    {
        EnterCriticalSection(parent->critical_section);
             state = parent->shared_data->network_state;
        LeaveCriticalSection(parent->critical_section);
    }

    if(state == NetworkState::CLIENT_INITIALIZED) return;

    auto new_key = [&](KeyEvent::KeyType key_type)->KeyEvent*
    {
        KeyEvent* key = new KeyEvent;
        key->key_type = key_type;
        key->id_tank.l_value = object_id;
        return key;
    };

    KeyEvent* key_event = nullptr;

    m_direction_time += dt;
    m_speed_time += dt;
    m_fire_time += dt;
    if(m_direction_time > m_keep_direction_time)
    {
//        cout << "Enemy Obj id: " << object_id << endl;
        m_direction_time = 0;
        m_keep_direction_time = rand() % 800 + 100;

        float p = static_cast<float>(rand()) / RAND_MAX;

        if(p < (type == ST_TANK_A ? 0.8 : 0.5) && target_position.x > 0 && target_position.y > 0)
        {
            int dx = target_position.x - (dest_rect.x + dest_rect.w / 2);
            int dy = target_position.y - (dest_rect.y + dest_rect.h / 2);

            p = static_cast<float>(rand()) / RAND_MAX;

            if(abs(dx) > abs(dy))
            {
                if(state == NetworkState::NONE)
                {
                    setDirection(p < 0.7 ? (dx < 0 ? D_LEFT : D_RIGHT) : (dy < 0 ? D_UP : D_DOWN));
                }
                else
                {
                    key_event = new_key(p < 0.7 ? (dx < 0 ? KeyEvent::KeyType::LEFT : KeyEvent::KeyType::RIGHT) : (dy < 0 ? KeyEvent::KeyType::UP : KeyEvent::KeyType::DOWN));
                }
            }
            else
            {
                if(state == NetworkState::NONE)
                {
                    setDirection(p < 0.7 ? (dy < 0 ? D_UP : D_DOWN) : (dx < 0 ? D_LEFT : D_RIGHT));
                }
                else
                {
                    key_event = new_key(p < 0.7 ? (dy < 0 ? KeyEvent::KeyType::UP : KeyEvent::KeyType::DOWN) : (dx < 0 ? KeyEvent::KeyType::LEFT : KeyEvent::KeyType::RIGHT));
                }
            }
        }
        else
        {
            if(state == NetworkState::NONE)
            {
                setDirection(static_cast<Direction>(rand() % 4));
            }
            else
            {
                key_event = new_key(static_cast<KeyEvent::KeyType>(rand() % 4));
            }
        }
    }

    if(m_speed_time > m_try_to_go_time)
    {
        m_speed_time = 0;
        m_try_to_go_time = 300;
        speed = default_speed;
    }

    if(m_fire_time > m_reload_time)
    {
        m_fire_time = 0;
        if(type == ST_TANK_D)
        {
            m_reload_time = rand() % 400;
            int dx = target_position.x - (dest_rect.x + dest_rect.w / 2);
            int dy = target_position.y - (dest_rect.y + dest_rect.h / 2);

            if(stop) fire();
            else
                switch (direction)
                {
                case D_UP:
                    if(dy < 0 && abs(dx) < dest_rect.w)
                    {
                        if(state == NetworkState::NONE)
                        {
                            fire();
                        }
                        else
                        {
                            key_event = new_key(KeyEvent::KeyType::FIRE);
                            EnterCriticalSection(parent->critical_section);
                                parent->shared_data->transmit_events.addEvent(key_event);
                            LeaveCriticalSection(parent->critical_section);
                        }
                    }

                    break;
                case D_RIGHT:
                    if(dx > 0 && abs(dy) < dest_rect.h)
                    {
                        if(state == NetworkState::NONE)
                        {
                            fire();
                        }
                        else
                        {
                            key_event = new_key(KeyEvent::KeyType::FIRE);
                            EnterCriticalSection(parent->critical_section);
                                parent->shared_data->transmit_events.addEvent(key_event);
                            LeaveCriticalSection(parent->critical_section);
                        }
                    }
                    break;
                case D_DOWN:
                    if(dy > 0 && abs(dx) < dest_rect.w)
                    {
                        if(state == NetworkState::NONE)
                        {
                            fire();
                        }
                        else
                        {
                            key_event = new_key(KeyEvent::KeyType::FIRE);
                            EnterCriticalSection(parent->critical_section);
                                parent->shared_data->transmit_events.addEvent(key_event);
                            LeaveCriticalSection(parent->critical_section);
                        }
                    }
                    break;
                case D_LEFT:
                    if(dx < 0 && abs(dy) < dest_rect.h)
                    {
                        if(state == NetworkState::NONE)
                        {
                            fire();
                        }
                        else
                        {
                            key_event = new_key(KeyEvent::KeyType::FIRE);
                            EnterCriticalSection(parent->critical_section);
                                parent->shared_data->transmit_events.addEvent(key_event);
                            LeaveCriticalSection(parent->critical_section);
                        }
                    }
                    break;
                }
        }
        else if(type == ST_TANK_D)
        {
            m_reload_time = rand() % 800;
            if(state == NetworkState::NONE)
            {
                fire();
            }
            else
            {
                key_event = new_key(KeyEvent::KeyType::FIRE);
                EnterCriticalSection(parent->critical_section);
                    parent->shared_data->transmit_events.addEvent(key_event);
                LeaveCriticalSection(parent->critical_section);
            }
        }
        else
        {
            m_reload_time = rand() % 1000;
            if(state == NetworkState::NONE)
            {
                fire();
            }
            else
            {
                key_event = new_key(KeyEvent::KeyType::FIRE);
                EnterCriticalSection(parent->critical_section);
                    parent->shared_data->transmit_events.addEvent(key_event);
                LeaveCriticalSection(parent->critical_section);
            }
        }
    }

    stop = false;

    if(parent != nullptr && key_event != nullptr)
    {
        EnterCriticalSection(parent->critical_section);
            parent->shared_data->transmit_events.addEvent(key_event);
        LeaveCriticalSection(parent->critical_section);
    }
}

void Enemy::destroy()
{
    lives_count--;
//    clearFlag(TSF_BONUS); //możliwe jednokrotne wypadnięcie bonusu
    if(lives_count <= 0)
    {
        lives_count = 0;
        Tank::destroy();
    }
}

unsigned Enemy::scoreForHit()
{
    if(lives_count > 0) return 50;
    return 100;
}
