#pragma once

#include <SFML/Audio.hpp>
#include <map>


class SoundHolder
{
    public:
        SoundHolder();
        ~SoundHolder();

        static sf::SoundBuffer& getSoundBuffer(std::string const& fileName);

    private:

        static SoundHolder* m_s_instance;

        std::map<std::string, sf::SoundBuffer> m_sounds;

        sf::SoundBuffer m_soundBuffer;
};


