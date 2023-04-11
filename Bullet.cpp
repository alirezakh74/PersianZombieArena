#include "Bullet.h"
#include "TextureHolder.h"
#include "SoundHolder.h"

#include <iostream>
#include <math.h>

//sf::SoundBuffer Bullet::m_soundBuffer = sf::SoundBuffer("sound/shoot.wav");
//sf::SoundBuffer* Bullet::m_soundBuffer->loadFromFile("sound/shoot.wav");

Bullet::Bullet()
{
    //ctor

    m_shape.setSize(sf::Vector2f(4, 4));

    m_sprite.setTexture(TextureHolder::getTexture("graphics/missiles/missile.png"));

    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);

    //m_soundMissile.setBuffer(SoundHolder::getSoundBuffer("sound/shoot.wav"));
    //m_soundMissile = (sf::Sound)SoundHolder::getSoundBuffer("sound/Gun.wav");
    //m_soundBuffer.loadFromFile("sound/Missile.wav");
    //m_soundMissile.setBuffer(m_soundBuffer);

    m_soundMissile.setBuffer(SoundHolder::getSoundBuffer("sound/Missile.wav"));
    m_soundShoot.setBuffer(SoundHolder::getSoundBuffer("sound/shoot.wav"));
    m_soundShoot.setVolume(20);
}

Bullet::~Bullet()
{
    //dtor
}

sf::RectangleShape Bullet::getShape()
{
    return m_shape;
}

sf::FloatRect Bullet::getPosition()
{
    return m_shape.getGlobalBounds();
}

void Bullet::shoot(float startX, float startY, float xTarget, float yTarget, float anglePlayer)
{
    m_soundShoot.play();

    m_inFlight = true;

    m_position.x = startX;
    m_position.y = startY;

    m_targetX = xTarget;
    m_targetY = yTarget;

    //angle = (atan2(m_targetY - m_position.y, m_targetX - m_position.x) * 180) / 3.141;

    angle = anglePlayer;

    std::cout << "angle = " << angle << std::endl;


    m_sprite.setRotation(angle);

    float radian = (angle * 3.141 / 180);

    m_distanceX = m_speed * cos(radian);
    m_distanceY = m_speed * sin(radian);

    std::cout << "distance x = " << m_distanceX << std::endl;
    std::cout << "distance y = " << m_distanceY << std::endl;

    float range = 1000;

    m_minX = startX - range;
    m_maxX = startX + range;
    m_minY = startY - range;
    m_maxY = startY + range;

    m_shape.setPosition(m_position);

    m_sprite.setPosition(m_position);


/*
    // Keep track of the bullet
	m_inFlight = true;
	m_position.x = startX;
	m_position.y = startY;

	// Calculate the gradient of the flight path
	float gradient = (startX - xTarget) / (startY - yTarget);

	// Any gradient less than zero needs to be negative
	if (gradient < 0)
	{
		gradient *= -1;
	}

	// Calculate the ratio between x and t
	float ratioXY = m_speed / (1 + gradient);

	// Set the "speed" horizontally and vertically
	m_distanceY = ratioXY;
	m_distanceX = ratioXY * gradient;

	// Point the bullet in the right direction
	if (xTarget < startX)
	{
		m_distanceX *= -1;
	}

	if (yTarget < startY)
	{
		m_distanceY *= -1;
	}

	// Finally, assign the results to the
	// member variables
	//m_XTarget = targetX;
	//m_YTarget = targetY;

	// Set a max range of 1000 pixels
	float range = 1000;
	m_minX = startX - range;
	m_maxX = startX + range;
	m_minY = startY - range;
	m_maxY = startY + range;

	// Position the bullet ready to be drawn
	m_shape.setPosition(m_position);
	m_sprite.setPosition(m_position);

	float angle = (atan2(yTarget - startY, xTarget - startX) * 180) / 3.141;

	m_sprite.setRotation(angle);

	*/

}

void Bullet::homingShoot(float startX, float startY, float enemyX, float enemyY, float anglePlayer)
{

    m_soundMissile.play();

    m_inFlight = true;
    m_homingMissile = true;

    m_position.x = startX;
    m_position.y = startY;

    m_targetX = enemyX;
    m_targetY = enemyY;

    angle = anglePlayer;

    m_sprite.setRotation(angle);
    m_sprite.setPosition(m_position);

    alpha = angle;

    teta = (atan2(m_targetY - m_position.y, m_targetX - m_position.x));

    float range = 1000;

    m_minX = startX - range;
    m_maxX = startX + range;
    m_minY = startY - range;
    m_maxY = startY + range;
}


void Bullet::stop()
{
    m_inFlight = false;
    m_homingMissile = false;
}

bool Bullet::isInFlight()
{
    return m_inFlight;
}

bool Bullet::isHomingMissile()
{
    return m_homingMissile;
}


sf::Sprite Bullet::getSprite()
{
    return m_sprite;
}


void Bullet::update(float elapsedTime, float enemyX, float enemyY)
{

    if(isInFlight() && !isHomingMissile())
    {
        m_position.x += m_distanceX * elapsedTime;
        m_position.y += m_distanceY * elapsedTime;
    }


    if(isInFlight() && isHomingMissile())
    {
        if(fabs(m_position.x - enemyX) < 20 && fabs(m_position.y - enemyY) < 20)
        {
            stop();
        }

        //teta = (atan2(m_position.y - enemyY,m_position.x - enemyX) * 180) / 3.141;
        teta = (atan2(enemyY - m_position.y,enemyX - m_position.x) * 180) / 3.141;

        while(teta >= 360)
        {
            teta -= 360;
        }

        while(teta < 0)
        {
            teta += 360;
        }


        //float n = (atan2(1, -1) * 180) / 3.141;

        //std::cout << "x = " << enemyX << std::endl;
        //std::cout << "y = " << enemyY << std::endl;

        alpha = m_sprite.getRotation();

        while(alpha >= 360)
        {
            alpha -= 360;
        }

        while(alpha < 0)
        {
            alpha += 360;
        }


        if(fabs(alpha - teta) >= maxDegreeRotate)
        {
            if(alpha > teta)
            {
                alpha -= maxDegreeRotate;
            }
            else
            {
                alpha += maxDegreeRotate;
            }
        }
            else
            {
                alpha = teta;
            }



        //std::cout << "a = " << alpha << std::endl;


        m_sprite.setRotation(alpha);

        //std::cout << "teta " << teta << std::endl;
        //std::cout << "alpha " << alpha << std::endl;

        float radian = (alpha * 3.141 / 180);

        m_distanceX = m_speed * cos(radian);
        m_distanceY = m_speed * sin(radian);

        m_position.x += m_distanceX * elapsedTime;
        m_position.y += m_distanceY * elapsedTime;

        //m_sprite.setPosition(m_position);

    }



    m_shape.setPosition(m_position);

    m_sprite.setPosition(m_position);

    if(m_position.x < m_minX || m_position.x > m_maxX ||
       m_position.y < m_minY || m_position.y > m_maxY)
    {
        m_inFlight = false;
    }
}

