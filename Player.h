#pragma once

#include <SFML/Graphics.hpp>

class Player
{
    private:
        const float START_SPEED = 200;
        const int START_HEALTH = 100;


        sf::Vector2f m_position;

        //sf::Texture m_texture;
        sf::Sprite m_sprite;

        bool m_leftPressed;
        bool m_rightPressed;
        bool m_upPressed;
        bool m_downPressed;

        float m_speed;

        int m_health;
        int m_maxHealth;

        sf::Time m_lastHit;

        int m_tileSize;

        sf::FloatRect m_arena;

        sf::Vector2f m_resolution;

    public:
        Player();

        void spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize);
        float getRotation();
        sf::Vector2f getCenter();
        sf::FloatRect getPosition();
        void resetPlayerState();
        bool hit(sf::Time timeHit);
        sf::Time getLastHitTime();

        sf::Sprite getSprite();

        void moveLeft();
        void moveRight();
        void moveUp();
        void moveDown();

        void stopLeft();
        void stopRight();
        void stopUp();
        void stopDown();

        void update(float deltaTime, sf::Vector2i mousePosition);

        int getHealth();

        void upgradeSpeed();

        void upgradeHealth();

        void increaseHealthLevel(int amount);

};
