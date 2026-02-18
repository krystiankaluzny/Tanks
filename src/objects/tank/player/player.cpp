#include "player.h"
#include "../../../appconfig.h"
#include "../../../soundconfig.h"
#include <iostream>

Player::Player(double x, double y, SpriteType type, std::vector<KeyCode> control_keys, InteractiveComponents interactive_components)
    : Tank(x, y, type, interactive_components),
      m_player_state_machine(new StateMachine())
{
    m_speed = 0;
    m_lives_count = 10;
    m_bullet_max_count = AppConfig::player_bullet_max_count;
    m_score = 0;
    star_count = 0;

    m_key_state_up = {control_keys[0], false};
    m_key_state_down = {control_keys[1], false};
    m_key_state_left = {control_keys[2], false};
    m_key_state_right = {control_keys[3], false};
    m_key_state_fire = {control_keys[4], false};

    m_player_state_machine->setState(new CreatingState(this));
}

Player::~Player()
{
    delete m_player_state_machine;
}

void Player::draw(Renderer &renderer)
{
    m_player_state_machine->draw(renderer);
}

void Player::drawPlayer(Renderer &renderer)
{
    Tank::draw(renderer);
}

void Player::handleKeyboardEvent(const KeyboardEvent &ev)
{
    if (ev.keyState() != KeyboardEvent::PRESSED && ev.keyState() != KeyboardEvent::RELEASED)
        return;

    KeyCode key_code = ev.keyCode();
    bool pressed = (ev.keyState() == KeyboardEvent::PRESSED);

    if (key_code == m_key_state_fire.key)
    {
        m_key_state_fire.pressed = pressed;
        return;
    }

    auto updateDir = [&](auto &state, Direction dir) -> bool
    {
        if (key_code == state.key)
        {
            state.pressed = pressed;
            if (pressed)
                m_last_pressed_direction = dir;
            return true;
        }
        return false;
    };

    if (updateDir(m_key_state_up, D_UP))
        return;
    if (updateDir(m_key_state_down, D_DOWN))
        return;
    if (updateDir(m_key_state_left, D_LEFT))
        return;
    if (updateDir(m_key_state_right, D_RIGHT))
        return;
}

void Player::update(Uint32 dt)
{
    m_player_state_machine->update(UpdateState{dt});
}

void Player::respawn()
{
    playSound(SoundConfig::PLAYER_RESPAWN);
    m_player_state_machine->setState(new CreatingState(this));
    activateShortShieldEffect();
}

void Player::hit()
{
    if (hasEffectByType(Tank::TankEffectType::SHIELD))
    {
        return;
    }

    if (hasEffectByType(Tank::TankEffectType::BOAT))
    {
        deleteEffectByType(Tank::TankEffectType::BOAT);
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
        m_player_state_machine->setState(new DestroyedState(this));
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

    m_player_state_machine->resetState(new CreatingState(this));
}

void Player::startPreview()
{
    m_player_state_machine->resetState(new PreviewState(this));
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