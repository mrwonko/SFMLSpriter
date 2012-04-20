#ifndef SPRITER_SFML_CHARACTER_HPP
#define SPRITER_SFML_CHARACTER_HPP

#include "API.hpp"
#include "TextureManager.hpp"

namespace SFMLSpriter
{

class SFML_SPRITER_API Character
{
public:
    Character(TextureManager& textureManager);

private:
    TextureManager& mTextureManager;
};

} //namespace SFMLSpriter

#endif
