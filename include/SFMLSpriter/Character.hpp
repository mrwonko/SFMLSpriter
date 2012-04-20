#ifndef SPRITER_SFML_CHARACTER_HPP
#define SPRITER_SFML_CHARACTER_HPP

#include "API.hpp"
#include "TextureManager.hpp"
#include <SFML/System/InputStream.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace SFMLSpriter
{
/** \brief A Spriter Character
    \note So far Spriter only supports one character per file, so file parsing is handled by this class for ease of use.
**/
class SFML_SPRITER_API Character : public sf::Drawable, public sf::Transformable
{
public:
    /** \brief Constructor
        \param textureManager The TextureManager to handle texture loading. Use a SimpleTextureManager if you don't have any special requirements. Must stay alive until until the Character object is destroyed.
    **/
    Character(TextureManager& textureManager);

    /** \brief Loads a given Spriter Character file
        \note Errors get written to sf::err()
        \param filename The name of the Spriter File
        \return Success
    **/
    const bool loadFromFile(const std::string& filename);

    /** \brief Loads a Spriter Character file in memory
        \note Errors get written to sf::err()
        \param xml The file content of the Spriter File
        \param rootDir directory to which image paths are relative (i.e. directory containing the Spriter File)
        \return Success
    **/
    const bool loadFromMemory(const std::string& xml, std::string rootDir = "");

    /** \brief Loads a Spriter Character from a given stream.
        \note Errors get written to sf::err()
        \param stream The InputStream from which to read
        \param rootDir directory to which image paths are relative (i.e. directory containing the Spriter File)
        \return Success
    **/
    const bool loadFromStream(sf::InputStream& stream, const std::string& rootDir = "");

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    TextureManager& mTextureManager;

};

} //namespace SFMLSpriter

#endif
