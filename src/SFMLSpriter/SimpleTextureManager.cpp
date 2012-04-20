#include <SFMLSpriter/SimpleTextureManager.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace SFMLSpriter
{
    SimpleTextureManager::SimpleTextureManager()
    {
        //ctor
    }
    SimpleTextureManager::~SimpleTextureManager()
    {
        //dtor
    }

    sf::Texture* SimpleTextureManager::getTexture(const std::string& name)
    {
        sf::Texture* tex = new sf::Texture();
        if(!loadTexture(*tex, name))
        {
            delete tex;
            return NULL;
        }
        return tex;
    }

    void SimpleTextureManager::releaseTexture(sf::Texture* tex)
    {
        delete tex;
    }

    const bool SimpleTextureManager::loadTexture(sf::Texture& tex, const std::string& name)
    {
        return tex.loadFromFile(name);
    }
}
