#include <SFMLSpriter/Animation.hpp>
#include <SFMLSpriter/Frame.hpp>
#include <rapidxml.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Config.hpp> //sf::Uint64
#include <cstdlib> //atoi

namespace SFMLSpriter
{

    Animation::Animation() :
        mTimeToNextFrame_ms(0)
    {
        //ctor
        mCurrentFrame = mKeyFrames.end();
    }

    void Animation::update(const sf::Uint64 deltaT_ms)
    {
        if(mKeyFrames.empty()) return;
        if(mCurrentFrame == mKeyFrames.end()) return;

        mTimeToNextFrame_ms -= deltaT_ms;
        while(mTimeToNextFrame_ms < 0)
        {
            ++mCurrentFrame;
            if(mCurrentFrame == mKeyFrames.end())
            {
                mCurrentFrame = mKeyFrames.begin();
            }
            mTimeToNextFrame_ms += mCurrentFrame->duration_ms;
        }
    }

    void Animation::restart()
    {
        mCurrentFrame = mKeyFrames.begin();
        if(mCurrentFrame == mKeyFrames.end()) return;
        mTimeToNextFrame_ms = mCurrentFrame->duration_ms;
    }

    void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(mCurrentFrame != mKeyFrames.end())
        {
            target.draw(*(mCurrentFrame->frame), states);
        }
    }

    const bool Animation::loadFromXMLNode(const rapidxml::xml_node<char>& node, const std::map<std::string, Frame*>& frames)
    {
        //read name
        rapidxml::xml_node<char>* nameNode = node.first_node("name");
        if(!nameNode || *nameNode->value() == '\0')
        {
            sf::err() << "Error: Invalid Spriter file: Animation contains no name!" << std::endl;
            return false;
        }
        mName.assign(nameNode->value());

        //load KeyFrames
        for(rapidxml::xml_node<char>* curNode = node.first_node("frame"); curNode; curNode = curNode->next_sibling("frame"))
        {
            rapidxml::xml_node<char>* nameNode = curNode->first_node("name");
            if(!nameNode)
            {
                //no need to clean up, this animation will be deleted due to failed loading anyway.
                sf::err() << "Error: Invalid Spriter file: Animation \"" << mName << "\" contains a Frame without name!" << std::endl;
                return false;
            }
            std::string frameName(nameNode->value());
            rapidxml::xml_node<char>* durationNode = curNode->first_node("duration");
            if(!durationNode)
            {
                sf::err() << "Error: Invalid Spriter file: Animation \"" << mName << "\" Frame \"" << frameName << "\" has no duration!" << std::endl;
                return false;
            }
            KeyFrame keyframe;
            keyframe.duration_ms = atof(durationNode->value());
            //only use KeyFrames with duration > 0
            if(keyframe.duration_ms <= 0)
            {
                sf::err() << "Warning: Invalid Spriter file: Animation \"" << mName << "\" Frame \"" << frameName << "\" has invalid duration " << keyframe.duration_ms << "!" << std::endl;
                continue;
            }
            //TODO: Remove once the time is *actually* in milliseconds, not in centiseconds.
            keyframe.duration_ms *= 10;
            std::map<std::string, Frame*>::const_iterator framePos = frames.find(frameName);
            if(framePos == frames.end())
            {
                sf::err()<< "Error: Invalid Spriter file: Animation \"" << mName << "\" references non-existant Frame \"" << frameName << "\"!" << std::endl;
                return false;
            }
            keyframe.frame = framePos->second;
            mKeyFrames.push_back(keyframe);
        }
        if(mKeyFrames.empty())
        {
            sf::err() << "Error: Invalid Spriter file: Animation \"" << mName << "\" contains no frames longer than 0ms!" << std::endl;
            return false;
        }
        mCurrentFrame = mKeyFrames.begin();
        return true;
    }

} // namespace SFMLSpriter
