#ifndef SFML_SPRITER_SIMPLE_TEXTURE_MANAGER_HPP
#define SFML_SPRITER_SIMPLE_TEXTURE_MANAGER_HPP

#include "TextureManager.hpp"

namespace SFMLSpriter
{

/** \brief A simple Texture Manager that creates a new texture each time getTexture() is called and deletes it on releaseTexture()

    This should be sufficient for single characters or when there are no common textures amongst characters and when sf::Texture::loadFromFile() works.
**/
class SFML_SPRITER_API SimpleTextureManager : public TextureManager
{
public:
    SimpleTextureManager();
    virtual ~SimpleTextureManager();
    virtual sf::Texture* getTexture(const std::string& name);
    virtual void releaseTexture(sf::Texture* texture);
protected:
    /** \brief How textures are loaded.

        If you don't need reference counting or some other kind of managing for textures but can't use sf::Texture::loadFromFile(), inherit from this class and overwrite this.
    **/
    virtual const bool loadTexture(sf::Texture& tex, const std::string& name);
};

}

#endif
