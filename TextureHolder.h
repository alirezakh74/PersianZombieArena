#pragma once

#include <SFML/Graphics.hpp>

#include <map>

class TextureHolder
{
    private:
        std::map<std::string, sf::Texture> m_textures;

        static TextureHolder* m_s_instance;

    public:
        TextureHolder();
        static sf::Texture& getTexture(std::string const& fileName);

};
