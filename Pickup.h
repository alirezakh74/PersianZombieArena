#pragma once

#include <SFML/Graphics.hpp>

class Pickup
{
    public:
        Pickup();
        Pickup(int pickupType);
        virtual ~Pickup();

        void setArena(sf::IntRect intRect);

        void spawn();

        sf::FloatRect getPosition();

        sf::Sprite getSprite();

        void update(float elapsedTime);

        bool isSpawned();

        int gotIt();

        void upgrade();


    private:
        const int HEALTH_START_VALUE = 50;
        const int AMMO_START_VALUE = 12;
        const int START_WAIT_TIME = 10;
        const int START_SECONDS_TO_LIVE = 5;

        sf::Sprite m_sprite;

        sf::IntRect m_arena;

        int m_value;

        // 0 -> helath && 1 ->> ammo
        int m_type;

        bool m_spawned;
        float m_secondsSinceSpawned;
        float m_secondsSinceDeSpawned;
        float m_secondsToLive;
        float m_secondsToWait;
};
