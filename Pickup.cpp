#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup()
{
    //ctor
}

Pickup::Pickup(int pickupType)
{
    m_type = pickupType;

    if(m_type == 1)
    {
        m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/health_pickup.png"));

        m_value = HEALTH_START_VALUE;
    }
    else
    {
        m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/ammo_pickup.png"));

        m_value = AMMO_START_VALUE;
    }

    m_sprite.setOrigin(25, 25);

    m_secondsToLive = START_SECONDS_TO_LIVE;
    m_secondsToWait = START_WAIT_TIME;
}

Pickup::~Pickup()
{
    //dtor
}

void Pickup::setArena(sf::IntRect intRect)
{
    m_arena.left = intRect.left + 50;
    m_arena.width = intRect.width - 50;
    m_arena.top = intRect.top + 50;
    m_arena.height = intRect.height - 50;

    spawn();
}

void Pickup::spawn()
{
    srand((int)time(0) / m_type);
    int x = (rand() % m_arena.width);
    srand((int)time(0) * m_type);
    int y = (rand() % m_arena.height);

    m_secondsSinceSpawned = 0;
    m_spawned = true;

    m_sprite.setPosition(x, y);
}

sf::FloatRect Pickup::getPosition()
{
    return m_sprite.getGlobalBounds();
}

sf::Sprite Pickup::getSprite()
{
    return m_sprite;
}

void Pickup::update(float elapsedTime)
{
    if(m_spawned)
    {
        m_secondsSinceSpawned += elapsedTime;
    }
    else
    {
        m_secondsSinceDeSpawned += elapsedTime;
    }

    if(m_secondsSinceSpawned > m_secondsToLive && m_spawned)
    {
        m_spawned = false;
        m_secondsSinceDeSpawned = 0;
    }

    if(m_secondsSinceDeSpawned > m_secondsToWait && !m_spawned)
    {
        spawn();
    }
}

bool Pickup::isSpawned()
{
    return m_spawned;
}

int Pickup::gotIt()
{
    m_spawned = false;
    m_secondsSinceDeSpawned = 0;
    return m_value;
}

void Pickup::upgrade()
{
    if(m_type == 1)
    {
        m_value += (HEALTH_START_VALUE * 0.5);
    }
    else
    {
        m_value += (AMMO_START_VALUE * 0.5);
    }

    m_secondsToLive += (m_secondsToLive / 10);
    m_secondsToWait -= (m_secondsToWait / 10);
}

