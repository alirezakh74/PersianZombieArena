#include "TextureHolder.h"

#include <assert.h>

TextureHolder* TextureHolder::m_s_instance = nullptr;

TextureHolder::TextureHolder()
{
    assert(m_s_instance == nullptr);

    m_s_instance = this;
}

sf::Texture& TextureHolder::getTexture(std::string const& fileName)
{
    auto& m = m_s_instance->m_textures;

    auto keyValuePair = m.find(fileName);

    if(keyValuePair != m.end())
    {
        return keyValuePair->second;
    }
    else
    {
        auto& texture = m[fileName];

        texture.loadFromFile(fileName);

        return texture;
    }
}
