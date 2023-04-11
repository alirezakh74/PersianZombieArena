#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Bullet
{
    public:
        Bullet();
        ~Bullet();

        sf::RectangleShape getShape();

        sf::Sprite getSprite();

        sf::FloatRect getPosition();

        void shoot(float startX, float startY, float xTarget, float yTarget, float anglePlayer);

        void homingShoot(float startX, float startY, float enemyX, float enemyY, float anglePlayer);

        void stop();

        bool isInFlight();

        bool isHomingMissile();

        void update(float elapsedTime, float enemyX, float enemyY);

    private:

        sf::RectangleShape m_shape;

        sf::Sprite m_sprite;

        sf::Vector2f m_position;

        sf::Sound m_soundMissile;
        sf::Sound m_soundShoot;

        bool m_inFlight = false;
        bool m_homingMissile = false;

        float m_speed = 100;
        float angle = 0;
        float alpha = 0;
        float teta = 0;
        float maxDegreeRotate = 0.5f;

        float m_distanceX = 0;
        float m_distanceY = 0;

        float m_maxX;
        float m_minX;
        float m_maxY;
        float m_minY;

        float m_targetX;
        float m_targetY;
};

