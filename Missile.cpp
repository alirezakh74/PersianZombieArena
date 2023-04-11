#include "Missile.h"
#include "TextureHolder.h"

Missile::Missile()
{
    //ctor
    m_sprite.setTexture(TextureHolder::getTexture("graphics/missiles/missile.png"));
}

Missile::~Missile()
{
    //dtor
}

sf::Sprite Missile::getSprite()
{
    return m_sprite;
}

sf::FloatRect Missile::getPosition()
{
    return m_sprite.getGlobalBounds();
}

bool Missile::isInFlight()
{
    return m_inFlight;
}

void Missile::shoot(float startX, float startY, float xTarget, float yTarget)
{
    m_inFlight = true;

    m_position.x = startX;
    m_position.y = startY;

    float gradient = (xTarget - startX) / (yTarget - startY);

    if(gradient < 0)
    {
        gradient *= -1;
    }

    float ratioXY = m_speed / (1 + gradient);

    m_distanceX = ratioXY * gradient;
    m_distanceY = ratioXY;

    if(xTarget < startX)
    {
        m_distanceX *= -1;
    }
    if(yTarget < startY)
    {
        m_distanceY *= -1;
    }

    float range = 1000;

    minX = startX - range;
    maxX = startX + range;

    minY = startY - range;
    maxY = startY - range;

    m_sprite.setPosition(m_position);

   /* float angle = (atan2(startY - yTarget, startX - xTarget) * 180) / 3.141;

    m_sprite.setRotation(angle); */

}

void Missile::stop()
{
    m_inFlight = false;
}

void Missile::update(float elapsedTime)
{
    m_position.x += m_distanceX * elapsedTime;
    m_position.y += m_distanceY * elapsedTime;

    if(m_position.x < minX || m_position.x > maxX ||
       m_position.y < minY || m_position.y > maxY)
    {
        m_inFlight = false;
    }
}

