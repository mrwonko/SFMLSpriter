#include <SFMLSpriter/Frame.hpp>
#include <SFMLSpriter/Character.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Err.hpp>
#include <rapidxml.hpp>
#include <cstdlib> //atoi

namespace SFMLSpriter
{
    Frame::Frame()
    {
        //ctor
    }

    Frame::~Frame()
    {
        releaseSprites();
    }

    void Frame::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for(SpriteList::const_iterator it = mSprites.begin(); it != mSprites.end(); ++it)
        {
            target.draw(**it, states);
        }
    }

    const bool Frame::loadFromXMLNode(const rapidxml::xml_node<char>& node, Character& character, const std::string& rootDir)
    {
        //read name
        rapidxml::xml_node<char>* nameNode = node.first_node("name");
        if(!nameNode)
        {
            sf::err() << "Error: Invalid Spriter file: Frame without name!" << std::endl;
            return false;
        }
        mName.assign(nameNode->value());
        //read and create sprites
        for(rapidxml::xml_node<char>* curNode = node.first_node("sprite"); curNode; curNode = curNode->next_sibling("sprite"))
        {
            //get <image>
            rapidxml::xml_node<char>* node = curNode->first_node("image");
            if(!node)
            {
                sf::err() << "Error: Invalid Spriter file: Frame \"" << mName << "\" lacks <image> node!" << std::endl;
                return false;
            }
            sf::Texture* tex = character.getTexture(rootDir + node->value());
            if(!tex)
            { //error should be printed by TextureManager
                return false;
            }

            //get <color> [0x000000 ; 0xffffff]
            node = curNode->first_node("color");
            if(!node)
            {
                sf::err() << "Error: Invalid Spriter file: Frame \"" << mName << "\" lacks <color> node!" << std::endl;
                return false;
            }
            int intColor = atoi(node->value());

            //get <opacity> [0.f ; 100.f]
            node = curNode->first_node("opacity");
            if(!node)
            {
                sf::err() << "Error: Invalid Spriter file: Frame \"" << mName << "\" lacks <opacity> node!" << std::endl;
                return false;
            }
            float opacity = atof(node->value());

            sf::Color color;
            color.r = (intColor & 0xff0000) >> 16;
            color.g = (intColor & 0x00ff00) >> 8;
            color.b = (intColor & 0x0000ff) >> 0;
            color.a = opacity / 100.f * 255.f;

            //get <angle> (in degrees, around upper left corner)
            node = curNode->first_node("angle");
            if(!node)
            {
                sf::err() << "Error: Invalid Spriter file: Frame \"" << mName << "\" lacks <angle> node!" << std::endl;
                return false;
            }
            float angle = atof(node->value());

            //get <width> (in pixels)
            node = curNode->first_node("width");
            if(!node)
            {
                sf::err() << "Error: Invalid Spriter file: Frame \"" << mName << "\" lacks <width> node!" << std::endl;
                return false;
            }
            float width = atof(node->value());

            //get <height> (in pixels)
            node = curNode->first_node("height");
            if(!node)
            {
                sf::err() << "Error: Invalid Spriter file: Frame \"" << mName << "\" lacks <height> node!" << std::endl;
                return false;
            }
            float height = atof(node->value());

            //get <xflip>
            node = curNode->first_node("xflip");
            if(!node)
            {
                sf::err() << "Error: Invalid Spriter file: Frame \"" << mName << "\" lacks <xflip> node!" << std::endl;
                return false;
            }
//            if(atoi(node->value())) width *= -1;

            //get <yflip>
            node = curNode->first_node("yflip");
            if(!node)
            {
                sf::err() << "Error: Invalid Spriter file: Frame \"" << mName << "\" lacks <yflip> node!" << std::endl;
                return false;
            }
//            if(atoi(node->value())) height *= -1;

            //get <x> (in pixels)
            node = curNode->first_node("x");
            if(!node)
            {
                sf::err() << "Error: Invalid Spriter file: Frame \"" << mName << "\" lacks <x> node!" << std::endl;
                return false;
            }
            float x = atof(node->value());

            //get <y> (in pixels)
            node = curNode->first_node("y");
            if(!node)
            {
                sf::err() << "Error: Invalid Spriter file: Frame \"" << mName << "\" lacks <y> node!" << std::endl;
                return false;
            }
            float y = atof(node->value());

            //create Sprite
            sf::Sprite* sprite = new sf::Sprite(*tex);
            //width/height is negative if the axis is flipped, in that case we need to reposition the origin to upper left
            sprite->setOrigin((width < 0 ? width : 0), (height < 0 ? height : 0));
            sprite->setPosition(x, y);
            sprite->setScale(width / float(tex->getSize().x), height / float(tex->getSize().y));
            sprite->setRotation(-angle);
            sprite->setColor(color);
            mSprites.push_back(sprite);
        }
        return true;
    }

    void Frame::releaseSprites()
    {
        for(SpriteList::iterator it = mSprites.begin(); it != mSprites.end(); ++it)
        {
            delete *it;
        }
        mSprites.clear();
    }
}
