#include "Player.h"
#include "TextureHolder.h"

Player::Player()
{
    m_speed = START_SPEED;
    m_health = START_HEALTH;
    m_maxHealth = START_HEALTH;

    //m_texture.loadFromFile("graphics/player.png");
    //m_texture.loadFromFile("graphics/sheykh1.png");
    //m_sprite.setTexture(m_texture);
    m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/player.png"));

    m_sprite.setOrigin(sf::Vector2f(25, 25));
    //m_sprite.setOrigin(sf::Vector2f(100, 100));
}

void Player::spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize)
{
    m_position.x = arena.width / 2;
    m_position.y = arena.height / 2;

    m_arena.top = arena.top;
    m_arena.left = arena.left;
    m_arena.width = arena.width;
    m_arena.height = arena.height;

    m_tileSize = tileSize;

    m_resolution.x = resolution.x;
    m_resolution.y = resolution.y;
}

void Player::resetPlayerState()
{
    m_speed = START_SPEED;
    m_health = START_SPEED;
    m_maxHealth = START_HEALTH;
}

bool Player::hit(sf::Time timeHit)
{
    if(timeHit.asMilliseconds() - m_lastHit.asMilliseconds() > 200)
    {
        m_lastHit = timeHit;
        m_health -= 10;
        return true;
    }
    else
    {
        return false;
    }
}

sf::Time Player::getLastHitTime()
{
    return m_lastHit;
}

float Player::getRotation()
{
    return m_sprite.getRotation();
}

sf::Vector2f Player::getCenter()
{
    return m_position;
}

sf::FloatRect Player::getPosition()
{
    return m_sprite.getGlobalBounds();
}


sf::Sprite Player::getSprite()
{
    return m_sprite;
}

int Player::getHealth()
{
    return m_health;
}

void Player::moveLeft()
{
    m_leftPressed = true;
}

void Player::moveRight()
{
    m_rightPressed = true;
}

void Player::moveUp()
{
    m_upPressed = true;
}

void Player::moveDown()
{
    m_downPressed = true;
}

void Player::stopLeft()
{
    m_leftPressed = false;
}

void Player::stopRight()
{
    m_rightPressed = false;
}

void Player::stopUp()
{
    m_upPressed = false;
}

void Player::stopDown()
{
    m_downPressed = false;
}

void Player::update(float deltaTime, sf::Vector2i mousePosition)
{
    if(m_upPressed)
    {
        m_position.y -= m_speed * deltaTime;
    }
    if(m_downPressed)
    {
        m_position.y += m_speed * deltaTime;
    }
    if(m_leftPressed)
    {
        m_position.x -= m_speed * deltaTime;
    }
    if(m_rightPressed)
    {
        m_position.x += m_speed * deltaTime;
    }

    m_sprite.setPosition(m_position);

    if(m_position.x > m_arena.width - m_tileSize)
    {
        m_position.x = m_arena.width - m_tileSize;
    }

    if(m_position.x < m_arena.left + m_tileSize)
    {
        m_position.x = m_arena.left + m_tileSize;
    }

    if(m_position.y > m_arena.height - m_tileSize)
    {
        m_position.y = m_arena.height - m_tileSize;
    }

    if(m_position.y < m_arena.top + m_tileSize)
    {
        m_position.y = m_arena.top + m_tileSize;
    }

    float angle = (atan2(mousePosition.y - m_resolution.y / 2,
                         mousePosition.x - m_resolution.x / 2) * 180) / 3.141;

    m_sprite.setRotation(angle);
}

void Player::upgradeSpeed()
{
    m_speed += (START_SPEED * 0.2);
}

void Player::upgradeHealth()
{
    m_maxHealth += (START_HEALTH * 0.2);
}

void Player::increaseHealthLevel(int amount)
{
    m_health += amount;

    if(m_health > m_maxHealth)
    {
        m_health = m_maxHealth;
    }
}





