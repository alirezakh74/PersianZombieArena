#include "SoundHolder.h"

#include <assert.h>

SoundHolder* SoundHolder::m_s_instance = nullptr;

SoundHolder::SoundHolder()
{
    //ctor

    assert(m_s_instance == nullptr);

    m_s_instance = this;
}

SoundHolder::~SoundHolder()
{
    //dtor
}

sf::SoundBuffer& SoundHolder::getSoundBuffer(std::string const& fileName)
{
    auto& m = m_s_instance->m_sounds;

    auto keyValuePair = m.find(fileName);

    if(keyValuePair != m.end())
    {
        return keyValuePair->second;
    }

    else
    {
        auto& soundBuffer = m[fileName];

        soundBuffer.loadFromFile(fileName);

        return soundBuffer;
    }

}

