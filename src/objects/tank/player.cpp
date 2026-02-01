#include "player.h"
#include "../../appconfig.h"
#include "../../soundconfig.h"
#include <iostream>

Player::Player(double x, double y, SpriteType type, std::vector<KeyCode> control_keys, InteractiveComponents interactive_components)
    : Tank(x, y, type, interactive_components)
{
    m_speed = 0;
    m_lives_count = 10;
    m_bullet_max_count = AppConfig::player_bullet_max_count;
    m_score = 0;
    star_count = 0;
    m_shield = new Object(x, y, ST_SHIELD);
    m_shield_time = 0;

    m_key_state_up = {control_keys[0], false};
    m_key_state_down = {control_keys[1], false};
    m_key_state_left = {control_keys[2], false};
    m_key_state_right = {control_keys[3], false};
    m_key_state_fire = {control_keys[4], false};

    moveToCreatingState();
}

void Player::handleKeyboardEvent(const KeyboardEvent &ev)
{
    KeyboardEvent::KeyState key_state = ev.keyState();
    if (key_state == KeyboardEvent::PRESSED || key_state == KeyboardEvent::RELEASED)
    {
        KeyCode key_code = ev.keyCode();
        bool pressed = (key_state == KeyboardEvent::PRESSED);

        if (key_code == m_key_state_up.key)
            m_key_state_up.pressed = pressed;
        else if (key_code == m_key_state_down.key)
            m_key_state_down.pressed = pressed;
        else if (key_code == m_key_state_left.key)
            m_key_state_left.pressed = pressed;
        else if (key_code == m_key_state_right.key)
            m_key_state_right.pressed = pressed;
        else if (key_code == m_key_state_fire.key)
            m_key_state_fire.pressed = pressed;
    }
}

void Player::update(Uint32 dt)
{
    Tank::update(dt);

    if (testFlag(TSF_ALIVE) && !testFlag(TSF_PREVIEW))
    {
        if (m_key_state_up.pressed)
        {
            setDirection(D_UP);
            m_speed = m_max_speed;
        }
        else if (m_key_state_down.pressed)
        {
            setDirection(D_DOWN);
            m_speed = m_max_speed;
        }
        else if (m_key_state_left.pressed)
        {
            setDirection(D_LEFT);
            m_speed = m_max_speed;
        }
        else if (m_key_state_right.pressed)
        {
            setDirection(D_RIGHT);
            m_speed = m_max_speed;
        }
        else
        {
            if (!testFlag(TSF_ON_ICE) || m_slip_time >= AppConfig::slip_time)
                m_speed = 0.0;
        }

        if (m_key_state_fire.pressed && m_fire_time > AppConfig::player_reload_time)
        {
            fire();
            m_fire_time = 0;
        }
    }

    m_fire_time += dt;

    if (testFlag(TSF_ALIVE))
        src_rect = m_sprite->rect.tiledOffset((testFlag(TSF_ON_ICE) ? m_tank_direction : m_moving_direction), m_current_frame + 2 * m_armor_count);
    else
        src_rect = m_sprite->rect.tiledOffset(0, m_current_frame + 2 * m_armor_count);

    m_blocked = false;
}

void Player::respawn()
{
    moveToCreatingState();
    m_shield_time = AppConfig::tank_shield_time / 2;
}

void Player::hit()
{
    if (testFlag(TSF_SHIELD))
    {
        return;
    }
    if (testFlag(TSF_BOAT))
    {
        clearFlag(TSF_BOAT);
        return;
    }

    if (star_count == 3)
    {
        changeStarCountBy(-1);
        playSound(SoundConfig::PLAYER_HIT);
    }
    else
    {
        changeStarCountBy(-3);
        Tank::destroy();
        playSound(SoundConfig::PLAYER_DESTROYED);
    }
}

void Player::moveToNextLevel()
{
    if (m_lives_count == 0)
    {
        m_lives_count = 2;
        m_armor_count = 1;
    }
    else
    {
        m_lives_count++;
    }

    moveToCreatingState();
}

Bullet *Player::fire()
{
    Bullet *b = Tank::fire();
    if (b != nullptr)
    {
        if (star_count > 0)
            b->increaseSpeed(1.3);
        if (star_count == 3)
            b->increaseDamage();

        playSound(SoundConfig::PLAYER_FIRED);
    }
    return b;
}

void Player::changeStarCountBy(int c)
{
    star_count += c;
    if (star_count > 3)
        star_count = 3;
    else if (star_count < 0)
        star_count = 0;

    m_armor_count = star_count;

    if (star_count >= 2 && c > 0)
        m_bullet_max_count++;
    else
        m_bullet_max_count = 2;

    if (star_count > 0)
        m_max_speed = AppConfig::tank_default_speed * 1.3;
    else
        m_max_speed = AppConfig::tank_default_speed;
}

void Player::resetKeyStates()
{
    m_key_state_up.pressed = false;
    m_key_state_down.pressed = false;
    m_key_state_left.pressed = false;
    m_key_state_right.pressed = false;
    m_key_state_fire.pressed = false;
}

void Player::addScore(unsigned points)
{
    m_score += points;
}

unsigned Player::score() const
{
    return m_score;
}

void Player::addLife()
{
    m_lives_count++;
}

unsigned Player::lives() const
{
    return m_lives_count;
}

double Player::speed() const
{
    return m_speed;
}

void Player::moveToCreatingState()
{
    if (type == ST_PLAYER_1)
    {
        pos_x = AppConfig::player_starting_point.at(0).x;
        pos_y = AppConfig::player_starting_point.at(0).y;
    }
    else
    {
        pos_x = AppConfig::player_starting_point.at(1).x;
        pos_y = AppConfig::player_starting_point.at(1).y;
    }

    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.h = m_sprite->rect.h;
    dest_rect.w = m_sprite->rect.w;

    setDirection(D_UP);
    creatingState();
    setFlag(TSF_SHIELD);
    resetKeyStates();

    for(auto b : bullets)
        b->to_erase = true;
}