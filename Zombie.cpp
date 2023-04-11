#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

Zombie::Zombie() : m_alive(true)
{
    //ctor
}

Zombie::~Zombie()
{
    //dtor
}

bool Zombie::hit()
{
    m_health--;

    if(m_health < 0)
    {
        m_alive = false;

        m_sprite.setTexture(TextureHolder::getTexture("graphics/blood.png"));

        return true;
    }

    return false;
}

bool Zombie::isAlive()
{
    return m_alive;
}

void Zombie::spawn(float startX, float startY, int type, int seed)
{
    switch(type)
    {
    case 0:
        m_sprite.setTexture(TextureHolder::getTexture("graphics/bloater-e.png"));
        //m_sprite.setTextureRect(sf::IntRect(5, 12, 67, 70));
        m_speed = 40;
        m_health = 5;
        break;

    case 1:
        m_sprite.setTexture(TextureHolder::getTexture("graphics/chaser-e.png"));
        //m_sprite.setTextureRect(sf::IntRect(8, 4, 46, 37));
        m_speed = 70;
        m_health = 1;
        break;

    case 2:
        m_sprite.setTexture(TextureHolder::getTexture("graphics/crawler-e.png"));
        //m_sprite.setTextureRect(sf::IntRect(0, 6, 0, 39));
        m_speed = 20;
        m_health = 3;
        break;

    case 3:
        m_sprite.setTexture(TextureHolder::getTexture("graphics/sheykh-e.png"));
        m_speed = 60;
        m_health = 6;
        break;
    }

    srand((int)time(0) * seed);

    float modifier = (rand() % MAX_VARRIANCE) + OFFSET;

    modifier /= 100;

    m_speed *= modifier;

    m_position.x = startX;
    m_position.y = startY;

    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);

    m_sprite.setPosition(m_position);
}

sf::FloatRect Zombie::getPosition()
{
    return m_sprite.getGlobalBounds();
}

sf::Sprite Zombie::getSprite()
{
    return m_sprite;
}

void Zombie::update(float elapsedTime, sf::Vector2f playerLocation)
{
    float playerX = playerLocation.x;
    float playerY = playerLocation.y;

    if(playerX < m_position.x)
    {
        m_position.x = m_position.x - m_speed * elapsedTime;
    }
    if(playerX > m_position.x)
    {
        m_position.x = m_position.x + m_speed * elapsedTime;
    }

    if(playerY < m_position.y)
    {
        m_position.y = m_position.y - m_speed * elapsedTime;
    }
    if(playerY > m_position.y)
    {
        m_position.y = m_position.y + m_speed * elapsedTime;
    }

    m_sprite.setPosition(m_position);

    float angle = (atan2(playerY - m_position.y, playerX - m_position.x) * 180) / 3.141;

    m_sprite.setRotation(angle);
}

sf::Vector2f Zombie::getCenter()
{
    return m_position;
}


