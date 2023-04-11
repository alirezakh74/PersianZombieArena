#pragma once

#include <SFML/Graphics.hpp>


class Missile
{
    public:
        Missile();
        ~Missile();

        sf::Sprite getSprite();

        sf::FloatRect getPosition();

        bool isInFlight();

        void shoot(float startX, float startY, float xTarget, float yTarget);

        void stop();

        void update(float elapsedTime);

    private:
        sf::Sprite m_sprite;

        sf::Vector2f m_position;

        float m_speed = 1000;

        bool m_inFlight;

        float m_distanceX;
        float m_distanceY;

        float maxX;
        float minX;
        float maxY;
        float minY;
};


