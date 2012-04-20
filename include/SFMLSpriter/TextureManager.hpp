#ifndef SFML_SPRITER_TEXTURE_MANAGER_HPP
#define SFML_SPRITER_TEXTURE_MANAGER_HPP

#include <string>
#include "API.hpp"

namespace sf
{
    class Texture;
}

namespace SFMLSpriter
{

/** \brief Interface for a class managing texture loading for Characters

    A character will only load each texture it uses once and destroy it once it's done with it, but if you have multiple characters sharing textures you might want to use a custom texture manager that makes sure every texture is only loaded once, even across characters.

    It is the TextureManager's responsibility to report image loading errors.
**/
class SFML_SPRITER_API TextureManager
{
protected:
    TextureManager() {}
public:
    virtual ~TextureManager() {}
    /** \brief Called to get a texture.

        If necessary the texture is loaded. A reference counter for it may be increased by 1.

        \return A pointer to the texture or NULL if loading failed **/
    virtual sf::Texture* getTexture(const std::string& name) = 0;
    /** \brief Called when a texture is no longer needed by a user.

        A reference counter may be decreased by 1 and the texture might get destroyed.
    **/
    virtual void releaseTexture(sf::Texture* texture) = 0;
};

}

#endif
