#include "level_environment.h"
#include "../../appconfig.h"
#include "../../soundconfig.h"
#include "../../objects/brick.h"
#include "../../objects/player.h"

#include <fstream>
#include <stdexcept>

LevelEnvironment::LevelEnvironment(int current_level, InteractiveComponents interactive_components)
    : InteractiveComponentsHolder(interactive_components)
{

    m_loaded_level = (current_level - 1) % AppConfig::levels_count + 1;
    m_eagle_protected = false;
    m_eagle_protection_time = 0;
    m_eagle_tile_rect = {AppConfig::map_size.w / 2 - 1, AppConfig::map_size.h - 2, 2, 2};

    m_map_outside_colision_left_rect = {-AppConfig::tile_size.w, -AppConfig::tile_size.h, AppConfig::tile_size.w, AppConfig::map_rect.h + 2 * AppConfig::tile_size.h};
    m_map_outside_colision_right_rect = {AppConfig::map_rect.w, -AppConfig::tile_size.h, AppConfig::tile_size.w, AppConfig::map_rect.h + 2 * AppConfig::tile_size.h};
    m_map_outside_colision_top_rect = {-AppConfig::tile_size.w, -AppConfig::tile_size.h, AppConfig::map_rect.w + 2 * AppConfig::tile_size.w, AppConfig::tile_size.h};
    m_map_outside_colision_bottom_rect = {-AppConfig::tile_size.w, AppConfig::map_rect.h, AppConfig::map_rect.w + 2 * AppConfig::tile_size.w, AppConfig::tile_size.h};

    loadLevel();
}

LevelEnvironment::~LevelEnvironment()
{
    for (auto &row : m_tile_objects)
    {
        for (auto obj : row)
        {
            if (obj != nullptr)
                delete obj;
        }
        row.clear();
    }
    m_tile_objects.clear();

    if (m_eagle != nullptr)
        delete m_eagle;
    m_eagle = nullptr;
}

void LevelEnvironment::drawFirstLayer(Renderer &renderer)
{
    for (const auto &row : m_tile_objects)
    {
        for (const auto obj : row)
        {
            if (obj != nullptr && obj->type != ST_BUSH)
                obj->draw(renderer);
        }
    }
}

void LevelEnvironment::drawSecondLayer(Renderer &renderer)
{
    for (const auto &row : m_tile_objects)
    {
        for (const auto obj : row)
        {
            if (obj != nullptr && (obj->type == ST_BUSH))
                obj->draw(renderer);
        }
    }

    if (m_eagle != nullptr)
        m_eagle->draw(renderer);
}

void LevelEnvironment::update(Uint32 dt)
{
    for (auto &row : m_tile_objects)
    {
        for (auto obj : row)
        {
            if (obj != nullptr)
                obj->update(dt);
        }
    }

    if (m_eagle != nullptr)
        m_eagle->update(dt);

    if (m_eagle_protected)
    {
        m_eagle_protection_time += dt;
        if (m_eagle_protection_time >= AppConfig::protect_eagle_time)
        {
            m_eagle_protected = false;
            m_eagle_protection_time = 0;

            changeEagleSurroundingsToBrickWalls();
        }

        if (m_eagle_protection_time > AppConfig::protect_eagle_time * 3 / 4)
        {
            if ((m_eagle_protection_time / AppConfig::bonus_blink_time) % 2)
            {
                changeEagleSurroundingsToBrickWalls();
            }
            else
            {
                changeEagleSurroundingsToStoneWalls();
            }
        }
        else if (m_eagle_protected)
        {
            changeEagleSurroundingsToStoneWalls();
        }
    }
}

void LevelEnvironment::destroyEagle()
{
    if (m_eagle != nullptr)
        m_eagle->destroy();
}

void LevelEnvironment::startEagleProtection()
{
    m_eagle_protected = true;
    m_eagle_protection_time = 0;

    changeEagleSurroundingsToStoneWalls();
}

void LevelEnvironment::checkCollisionTankWithLevel(Tank *tank, Uint32 dt)
{
    if (tank->to_erase)
        return;

    checkCollisionTankWithEnteringEnv(tank, dt);
    checkCollisionTankWithLeavingEnv(tank, dt);

    Rect intersect_rect;
    Rect pr = tank->nextCollisionRect(dt);

    intersect_rect = m_eagle->collision_rect.intersection(pr);
    if (intersect_rect.isNotEmpty())
        tank->collide(intersect_rect);

    intersect_rect = m_map_outside_colision_top_rect.intersection(pr);
    if (intersect_rect.isNotEmpty())
        tank->collide(intersect_rect);

    intersect_rect = m_map_outside_colision_bottom_rect.intersection(pr);
    if (intersect_rect.isNotEmpty())
        tank->collide(intersect_rect);

    intersect_rect = m_map_outside_colision_left_rect.intersection(pr);
    if (intersect_rect.isNotEmpty())
        tank->collide(intersect_rect);

    intersect_rect = m_map_outside_colision_right_rect.intersection(pr);
    if (intersect_rect.isNotEmpty())
        tank->collide(intersect_rect);
}

void LevelEnvironment::checkCollisionBulletWithLevel(Bullet *bullet)
{
    if (bullet == nullptr)
        return;
    if (bullet->isColide())
        return;

    bool is_player_bullet = (dynamic_cast<Player *>(bullet->owner()) != nullptr);
    int row_start, row_end;
    int column_start, column_end;

    //========================kolizja z elementami mapy========================
    switch (bullet->direction())
    {
    case D_UP:
        row_start = row_end = bullet->collision_rect.y / AppConfig::tile_size.h;
        column_start = bullet->collision_rect.x / AppConfig::tile_size.w;
        column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / AppConfig::tile_size.w;
        break;
    case D_RIGHT:
        column_start = column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / AppConfig::tile_size.w;
        row_start = bullet->collision_rect.y / AppConfig::tile_size.h;
        row_end = (bullet->collision_rect.y + bullet->collision_rect.h) / AppConfig::tile_size.h;
        break;
    case D_DOWN:
        row_start = row_end = (bullet->collision_rect.y + bullet->collision_rect.h) / AppConfig::tile_size.h;
        column_start = bullet->collision_rect.x / AppConfig::tile_size.w;
        column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / AppConfig::tile_size.w;
        break;
    case D_LEFT:
        column_start = column_end = bullet->collision_rect.x / AppConfig::tile_size.w;
        row_start = bullet->collision_rect.y / AppConfig::tile_size.h;
        row_end = (bullet->collision_rect.y + bullet->collision_rect.h) / AppConfig::tile_size.h;
        break;
    }
    if (column_start < 0)
        column_start = 0;
    if (row_start < 0)
        row_start = 0;
    if (column_end >= AppConfig::map_size.w)
        column_end = AppConfig::map_size.w - 1;
    if (row_end >= AppConfig::map_size.h)
        row_end = AppConfig::map_size.h - 1;

    Rect intersect_rect;
    Object *o;

    bool shoudl_destroy_bulet = false;
    Rect &br = bullet->collision_rect;

    for (int i = row_start; i <= row_end; i++)
        for (int j = column_start; j <= column_end; j++)
        {
            o = m_tile_objects.at(i).at(j);
            if (o == nullptr)
                continue;
            if (o->type == ST_ICE || o->type == ST_WATER)
                continue;

            Rect &lr = o->collision_rect;
            intersect_rect = lr.intersection(br);

            if (intersect_rect.isNotEmpty())
            {
                if (bullet->isDamageIncreased())
                {
                    delete o;
                    m_tile_objects.at(i).at(j) = nullptr;
                }
                else if (o->type == ST_BRICK_WALL)
                {
                    Brick *brick = dynamic_cast<Brick *>(o);
                    brick->bulletHit(bullet->direction());
                    if (is_player_bullet)
                            playSound(SoundConfig::BULLET_HIT_BRICK);
                    if (brick->to_erase)
                    {
                        delete brick;
                        m_tile_objects.at(i).at(j) = nullptr;
                    }
                }
                else if (o->type == ST_STONE_WALL)
                {
                    // stone wall - do nothing, just play sound
                    if (is_player_bullet)
                        playSound(SoundConfig::BULLET_HIT_WALL);
                }
                // Do not stop bullet on bush, it allows to destroy all of bushes on the bullet way,
                // when damages are increased
                if (o->type != ST_BUSH)
                {
                    shoudl_destroy_bulet = true;
                }
            }
        }

    if (shoudl_destroy_bulet)
    {
        bullet->destroy();
    }

    //========================kolizja z granicami mapy========================
    if (br.x < 0 || br.y < 0 || br.x + br.w > AppConfig::map_rect.w || br.y + br.h > AppConfig::map_rect.h)
    {
        bullet->destroy();
        if (is_player_bullet)
            playSound(SoundConfig::BULLET_HIT_WALL);
    }
}

bool LevelEnvironment::checkCollisionBulletWithEagle(Bullet *bullet)
{
    if (bullet == nullptr)
        return false;
    if (bullet->isColide())
        return false;

    Rect intersect_rect = m_eagle->collision_rect.intersection(bullet->collision_rect);
    if (intersect_rect.isNotEmpty())
    {
        bullet->destroy();
        m_eagle->destroy();
        playSound(SoundConfig::EAGLE_DESTROYED);
        return true;
    }
    return false;
}

bool LevelEnvironment::checkCollisionWithEagle(Rect &rect)
{
    Rect intersect_rect = m_eagle->collision_rect.intersection(rect);
    return intersect_rect.isNotEmpty();
}

Point LevelEnvironment::getEagleCenter() const
{
    return {m_eagle->pos_x + (int)(m_eagle->dest_rect.w / 2), m_eagle->pos_y + (int)(m_eagle->dest_rect.h / 2)};
}

void LevelEnvironment::checkCollisionTankWithEnteringEnv(Tank *tank, Uint32 dt)
{
    int row_start, row_end;
    int column_start, column_end;

    Object *o;

    // we check the range of tiles in front of the tank based on its direction
    switch (tank->movingDirection())
    {
    case D_UP:
        row_end = tank->collision_rect.y / AppConfig::tile_size.h;
        row_start = row_end - 1;
        column_start = tank->collision_rect.x / AppConfig::tile_size.w - 1;
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_size.w + 1;
        break;
    case D_RIGHT:
        column_start = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_size.w;
        column_end = column_start + 1;
        row_start = tank->collision_rect.y / AppConfig::tile_size.h - 1;
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / AppConfig::tile_size.h + 1;
        break;
    case D_DOWN:
        row_start = (tank->collision_rect.y + tank->collision_rect.h) / AppConfig::tile_size.h;
        row_end = row_start + 1;
        column_start = tank->collision_rect.x / AppConfig::tile_size.w - 1;
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_size.w + 1;
        break;
    case D_LEFT:
        column_end = tank->collision_rect.x / AppConfig::tile_size.w;
        column_start = column_end - 1;
        row_start = tank->collision_rect.y / AppConfig::tile_size.h - 1;
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / AppConfig::tile_size.h + 1;
        break;
    }
    if (column_start < 0)
        column_start = 0;
    if (row_start < 0)
        row_start = 0;
    if (column_end >= AppConfig::map_size.w)
        column_end = AppConfig::map_size.w - 1;
    if (row_end >= AppConfig::map_size.h)
        row_end = AppConfig::map_size.h - 1;

    Rect pr = tank->nextCollisionRect(dt);
    Rect intersect_rect;

    bool on_ice = false;

    for (int i = row_start; i <= row_end; i++)
    {
        for (int j = column_start; j <= column_end; j++)
        {
            o = m_tile_objects.at(i).at(j);
            if (o == nullptr)
                continue;
            if (o->type == ST_BUSH)
                continue;
            if (tank->testFlag(Tank::TSF_BOAT) && o->type == ST_WATER)
                continue;

            Rect &lr = o->collision_rect;

            intersect_rect = lr.intersection(pr);
            if (intersect_rect.isNotEmpty())
            {
                if (o->type == ST_ICE)
                {
                    if (intersect_rect.w > 10 && intersect_rect.h > 10)
                        on_ice = true;
                    continue;
                }
                else
                    tank->collide(intersect_rect);
                break;
            }
        }
    }

    if (on_ice)
    {
        tank->setFlag(Tank::TSF_ON_ICE);
    }
}

void LevelEnvironment::checkCollisionTankWithLeavingEnv(Tank *tank, Uint32 dt)
{
    int row_start, row_end;
    int column_start, column_end;

    Object *o;

    // we check the range of tiles in rear of the tank based on its direction
    switch (tank->movingDirection())
    {
    case D_UP:
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / AppConfig::tile_size.h;
        row_start = row_end - 1;
        column_start = tank->collision_rect.x / AppConfig::tile_size.w - 1;
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_size.w + 1;
        break;
    case D_RIGHT:
        column_start = (tank->collision_rect.x) / AppConfig::tile_size.w;
        column_end = column_start + 1;
        row_start = tank->collision_rect.y / AppConfig::tile_size.h - 1;
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / AppConfig::tile_size.h + 1;
        break;
    case D_DOWN:
        row_start = (tank->collision_rect.y) / AppConfig::tile_size.h;
        row_end = row_start + 1;
        column_start = tank->collision_rect.x / AppConfig::tile_size.w - 1;
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_size.w + 1;
        break;
    case D_LEFT:
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / AppConfig::tile_size.w;
        column_start = column_end - 1;
        row_start = tank->collision_rect.y / AppConfig::tile_size.h - 1;
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / AppConfig::tile_size.h + 1;
        break;
    }
    if (column_start < 0)
        column_start = 0;
    if (row_start < 0)
        row_start = 0;
    if (column_end >= AppConfig::map_size.w)
        column_end = AppConfig::map_size.w - 1;
    if (row_end >= AppConfig::map_size.h)
        row_end = AppConfig::map_size.h - 1;

    Rect pr = tank->nextCollisionRect(dt);
    Rect intersect_rect;

    bool on_ice = false;

    for (int i = row_start; i <= row_end; i++)
    {
        for (int j = column_start; j <= column_end; j++)
        {
            o = m_tile_objects.at(i).at(j);
            if (o == nullptr)
                continue;

            if (o->type == ST_ICE)
            {
                Rect &lr = o->collision_rect;
                intersect_rect = lr.intersection(pr);
                if (intersect_rect.isNotEmpty())
                {
                    if (intersect_rect.w > 10 && intersect_rect.h > 10)
                        on_ice = true;
                }
            }
        }
    }

    if (on_ice)
    {
        tank->setFlag(Tank::TSF_ON_ICE);
    }
    else
    {
        tank->clearFlag(Tank::TSF_ON_ICE);
    }
}

void LevelEnvironment::loadLevel()
{
    std::ifstream level_file;
    std::string level_file_name = AppConfig::levels_path + std::to_string(m_loaded_level);
    level_file.open(level_file_name);
    if (!level_file.is_open())
    {
        throw std::runtime_error("Cannot open level file: " + level_file_name);
    }

    m_tile_objects.clear();

    std::string line;
    size_t row_index = 0;

    while (!level_file.eof())
    {
        std::getline(level_file, line);
        std::vector<Object *> row;
        for (size_t col_index = 0; col_index < line.length() && col_index < (size_t)AppConfig::map_size.w; col_index++)
        {
            // Skip eagle area
            if (col_index >= (size_t)m_eagle_tile_rect.x && col_index < (size_t)m_eagle_tile_rect.x + (size_t)m_eagle_tile_rect.w &&
                row_index >= (size_t)m_eagle_tile_rect.y && row_index < (size_t)m_eagle_tile_rect.y + (size_t)m_eagle_tile_rect.h)
            {
                row.push_back(nullptr);
                continue;
            }

            Object *obj = nullptr;
            switch (line.at(col_index))
            {
            case '#':
                obj = new Brick(col_index * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h);
                break;
            case '@':
                obj = new Object(col_index * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h, ST_STONE_WALL);
                break;
            case '%':
                obj = new Object(col_index * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h, ST_BUSH);
                break;
            case '~':
                obj = new Object(col_index * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h, ST_WATER);
                break;
            case '-':
                obj = new Object(col_index * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h, ST_ICE);
                break;
            default:
                obj = nullptr;
            }
            row.push_back(obj);
        }
        m_tile_objects.push_back(row);
        row_index++;

        if (row_index >= (size_t)AppConfig::map_size.h)
            break;
    }

    level_file.close();

    m_eagle = new Eagle(m_eagle_tile_rect.x * AppConfig::tile_size.w, m_eagle_tile_rect.y * AppConfig::tile_size.h);
}

void LevelEnvironment::changeEagleSurroundingsToStoneWalls()
{
    for (int i = 0; i < m_eagle_tile_rect.h + 1; i++)
    {
        int row_index = AppConfig::map_size.h - i - 1;
        if (m_tile_objects.at(row_index).at(m_eagle_tile_rect.x - 1) != nullptr)
            delete m_tile_objects.at(row_index).at(m_eagle_tile_rect.x - 1);
        m_tile_objects.at(row_index).at(m_eagle_tile_rect.x - 1) = new Object((m_eagle_tile_rect.x - 1) * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h, ST_STONE_WALL);

        if (m_tile_objects.at(row_index).at(m_eagle_tile_rect.x + m_eagle_tile_rect.w) != nullptr)
            delete m_tile_objects.at(row_index).at(m_eagle_tile_rect.x + m_eagle_tile_rect.w);
        m_tile_objects.at(row_index).at(m_eagle_tile_rect.x + m_eagle_tile_rect.w) = new Object((m_eagle_tile_rect.x + m_eagle_tile_rect.w) * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h, ST_STONE_WALL);
    }
    for (int i = m_eagle_tile_rect.x - 1; i < m_eagle_tile_rect.x + m_eagle_tile_rect.w + 1; i++)
    {
        int row_index = AppConfig::map_size.h - m_eagle_tile_rect.h - 1;
        if (m_tile_objects.at(row_index).at(i) != nullptr)
            delete m_tile_objects.at(row_index).at(i);
        m_tile_objects.at(row_index).at(i) = new Object(i * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h, ST_STONE_WALL);
    }
}

void LevelEnvironment::changeEagleSurroundingsToBrickWalls()
{
    for (int i = 0; i < m_eagle_tile_rect.h + 1; i++)
    {
        int row_index = AppConfig::map_size.h - i - 1;
        if (m_tile_objects.at(row_index).at(m_eagle_tile_rect.x - 1) != nullptr)
            delete m_tile_objects.at(row_index).at(m_eagle_tile_rect.x - 1);
        m_tile_objects.at(row_index).at(m_eagle_tile_rect.x - 1) = new Brick((m_eagle_tile_rect.x - 1) * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h);

        if (m_tile_objects.at(row_index).at(m_eagle_tile_rect.x + m_eagle_tile_rect.w) != nullptr)
            delete m_tile_objects.at(row_index).at(m_eagle_tile_rect.x + m_eagle_tile_rect.w);
        m_tile_objects.at(row_index).at(m_eagle_tile_rect.x + m_eagle_tile_rect.w) = new Brick((m_eagle_tile_rect.x + m_eagle_tile_rect.w) * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h);
    }
    for (int i = m_eagle_tile_rect.x - 1; i < m_eagle_tile_rect.x + m_eagle_tile_rect.w + 1; i++)
    {
        int row_index = AppConfig::map_size.h - m_eagle_tile_rect.h - 1;
        if (m_tile_objects.at(row_index).at(i) != nullptr)
            delete m_tile_objects.at(row_index).at(i);
        m_tile_objects.at(row_index).at(i) = new Brick(i * AppConfig::tile_size.w, row_index * AppConfig::tile_size.h);
    }
};
