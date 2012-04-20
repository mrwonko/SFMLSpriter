#ifndef SPRITER_SFML_CHARACTER_HPP
#define SPRITER_SFML_CHARACTER_HPP

#include "API.hpp"
#include "TextureManager.hpp"
#include <SFML/System/InputStream.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <map>

namespace SFMLSpriter
{
//forward declarations
class Frame;
class Animation;

/** \brief A Spriter Character
    \note So far Spriter only supports one character per file, so file parsing is handled by this class for ease of use.

    Textures are loaded using the TextureManager supplied to the constructor.
**/
class SFML_SPRITER_API Character : public sf::Drawable, public sf::Transformable
{
public:
    /** \brief Constructor
        \param textureManager The TextureManager to handle texture loading. Use a SimpleTextureManager if you don't have any special requirements. Must stay alive until until the Character object is destroyed.
    **/
    Character(TextureManager& textureManager);

    /** \brief Destructor - uses the TextureManager supplied in the constructor to release all textures
    **/
    ~Character();

    /** \brief Loads a given Spriter Character file and starts playing the first animation
        \note Errors get written to sf::err()
        \param filename The name of the Spriter File
        \return Success
    **/
    const bool loadFromFile(const std::string& filename);

    /** \brief Loads a Spriter Character file in memory and starts playing the first animation
        \note Errors get written to sf::err()
        \param xml The file content of the Spriter File
        \param rootDir directory to which image paths are relative (i.e. directory containing the Spriter File)
        \return Success
    **/
    const bool loadFromMemory(const std::string& xml, std::string rootDir = "");

    /** \brief Loads a Spriter Character from a given stream and starts playing the first animation
        \note Errors get written to sf::err()
        \param stream The InputStream from which to read
        \param rootDir directory to which image paths are relative (i.e. directory containing the Spriter File)
        \return Success
    **/
    const bool loadFromStream(sf::InputStream& stream, const std::string& rootDir = "");

    /** \brief Returns the bounding box of the character in the current animation (0,0,0,0 if non available)
    **/
    const sf::FloatRect& getBoundingBox() const { return mBoundingBox; }

    /** \brief Sets the current animation to the given one, if it exists.

        If the animation is not found, the last one is kept.
        \return Success, i.e. whether such an animation exists.
    */
    const bool setAnimation(const std::string& animationName);

    /** \brief Updates, i.e. advances the current animation by the given time (must be positive)
        \param deltaT_ms The time to advance, in milliseconds
    **/
    void update(const sf::Uint64 deltaT_ms);

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    TextureManager& mTextureManager;

    typedef std::map<std::string, Frame*> FrameMap;
    FrameMap mFrames;
    void releaseFrames();

    typedef std::map<std::string, Animation*> AnimationMap;
    AnimationMap mAnimations;
    void releaseAnimations();
    AnimationMap::iterator mCurrentAnimation;

    //Frame does the texture loading, hence needs to access this.
    friend class Frame;

    typedef std::map<std::string, sf::Texture*> TextureMap;
    TextureMap mTextures;
    // If the texture has already been required, returns the previously loaded Texture (or a NULL pointer if it failed). Otherwise uses the TextureManager to load it and returns the result.
    sf::Texture* getTexture(const std::string& filename);
    // Releases all textures via the TextureManager
    void releaseTextures();

    sf::FloatRect mBoundingBox;
};

} //namespace SFMLSpriter

#endif
