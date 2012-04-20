#ifndef SFMLSPRITER_ANIMATION_H
#define SFMLSPRITER_ANIMATION_H

#include <string>
#include <list>
#include <map>
#include <SFML/Config.hpp> //for Int64
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>

//forward declarations
namespace rapidxml
{
    template<class Ch> class xml_node;
}

namespace SFMLSpriter
{
class Character;
class Frame;

class Animation : public sf::Drawable
{
    public:
        Animation();

        const std::string& getName() const { return mName; }

        void update(const sf::Uint64 deltaT_ms);

        /** \brief Restarts the animation from the first frame
        **/
        void restart();

        const bool isEmpty() const { return mKeyFrames.empty(); }

        const bool loadFromXMLNode(const rapidxml::xml_node<char>& node, const std::map<std::string, Frame*>& frames);

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        std::string mName;

        struct KeyFrame
        {
            Frame* frame;
            float duration_ms;
        };

        typedef std::list<KeyFrame> KeyFrameList;
        KeyFrameList mKeyFrames;
        KeyFrameList::iterator mCurrentFrame;
        float mTimeToNextFrame_ms;
};

} // namespace SFMLSpriter

#endif // SFMLSPRITER_ANIMATION_H
