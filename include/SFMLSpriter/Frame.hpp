#ifndef SFML_SPRITER_FRAME_HPP
#define SFML_SPRITER_FRAME_HPP

#include <string>
#include <list>
#include <SFML/Graphics/Drawable.hpp>

//forward declarations
namespace rapidxml
{
    template<class Ch> class xml_node;
}

namespace sf
{
    class Sprite;
}

namespace SFMLSpriter
{
    class Character;

    class Frame : public sf::Drawable
    {
    public:
        Frame();
        ~Frame();

        const bool loadFromXMLNode(const rapidxml::xml_node<char>& node, Character& chararacter, const std::string& rootDir);

        const std::string& getName() const { return mName; }

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        std::string mName;
        typedef std::list<sf::Sprite*> SpriteList;
        SpriteList mSprites;
        void releaseSprites();
    };

}

#endif // SFML_SPRITER_FRAME_HPP
