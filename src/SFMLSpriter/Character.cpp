#include <SFMLSpriter/Character.hpp>
#include <SFMLSpriter/Frame.hpp>
#include <SFMLSpriter/Animation.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <rapidxml.hpp>
#include <fstream>
#include <vector>
#include <streambuf>

namespace SFMLSpriter
{

    Character::Character(TextureManager& textureManager) :
        mTextureManager(textureManager)
    {
        //ctor
        mCurrentAnimation = mAnimations.end();
    }

    Character::~Character()
    {
        releaseTextures();
        releaseFrames();
        releaseAnimations();
    }

    const bool Character::loadFromFile(const std::string& filename)
    {
        std::ifstream file(filename.c_str());
        if(file.fail())
        {
            sf::err() << "Could not open \"" << filename << "\"!" << std::endl;
            return false;
        }
        std::string contents;
        //read file size for up-front-allocation
        file.seekg(0, std::ios::end);
        contents.reserve(file.tellg());
        //move cursor back to front
        file.seekg(0, std::ios::beg);
        //read contents
        contents.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        if(file.fail())
        {
            sf::err() << "Could not read \"" << filename << "\"!" << std::endl;
            return false;
        }
        //get directory name from full path (for texture loading) by searching for (back)slashes
        std::string::size_type dirEnd = filename.find_last_of("\\/");
        if(dirEnd == std::string::npos) // no backslashes? no subdirectory then.
        {
            return loadFromMemory(contents);
        }
        else
        {
            return loadFromMemory(contents, filename.substr(0, dirEnd));
        }
    }

    const bool Character::loadFromStream(sf::InputStream& stream, const std::string& rootDir)
    {
        if(stream.seek(0) == -1)
        {
            sf::err() << "Could not seek beginning of stream!" << std::endl;
            return false;
        }
        sf::Int64 streamSize = stream.getSize();
        if(streamSize == -1)
        {
            sf::err() << "Could not get stream size!" << std::endl;
            return false;
        }
        char* buffer = new char[streamSize];
        sf::Int64 read = stream.read(buffer, streamSize);
        std::string contents(buffer, read);
        delete[] buffer;
        if(stream.tell() == -1)
        {
            sf::err() << "Error reading from stream!" << std::endl;
            return false;
        }
        return loadFromMemory(contents, rootDir);
    }

    const bool Character::loadFromMemory(const std::string& xmlString, std::string rootDir)
    {
        //sanitize rootDir - it needs a trailing slash, unless it's empty
        if(!rootDir.empty())
        {
            const char& lastChar = rootDir[rootDir.length()-1];
            if(lastChar != '/' && lastChar != '\\')
            {
                rootDir += "/";
            }
        }
        //create a copy of the xmlString that may be modified (which rapidxml would like to so it can zero-terminate strings)
        std::vector<char> xmlStringWriteable;
        xmlStringWriteable.reserve(xmlString.length() + 1);
        xmlStringWriteable.assign(xmlString.begin(), xmlString.end());
        xmlStringWriteable.push_back('\0');

        //parse XML
        try //xml throws on error
        {
            rapidxml::xml_document<char> xmlDoc;
            xmlDoc.parse<0>(xmlStringWriteable.data()); //0 = default parse flags
            rapidxml::xml_node<char>* rootNode = xmlDoc.first_node("spriterdata");
            if(!rootNode)
            {
                sf::err() << "Error: No valid Spriter file (lacks spriterdata node)!" << std::endl;
                return false;
            }
            //load frames
            for(rapidxml::xml_node<char>* node = rootNode->first_node("frame"); node; node = node->next_sibling("frame"))
            {
                Frame* frame = new Frame();
                if(!frame->loadFromXMLNode(*node, *this, rootDir))
                {
                    delete frame;
                    releaseFrames();
                    releaseTextures();
                    return false;
                }
                //check if there's a frame of this name yet
                if(mFrames.find(frame->getName()) != mFrames.end())
                {
                    sf::err() << "Warning: Duplicate Frame \"" << frame->getName() << "\" in Spriter file!" << std::endl;
                    delete frame;
                    continue;
                }
                mFrames[frame->getName()] = frame;
            }
            //any frames at all?
            if(mFrames.empty())
            {
                sf::err() << "Warning: No frames in Spriter file!" << std::endl;
            }
            //find character - in the beta file format there should be only one
            rapidxml::xml_node<char>* characterNode = rootNode->first_node("char");
            if(!characterNode)
            {
                sf::err() << "Warning: No character in Spriter file!" << std::endl;
                releaseFrames();
                releaseTextures();
                return true;
            }
            if(characterNode->next_sibling("char"))
            {
                sf::err() << "Warning: More than one character in Spriter file! Only first one is used." << std::endl;
            }
            //load animations
            for(rapidxml::xml_node<char>* node = characterNode->first_node("anim"); node; node = node->next_sibling("anim"))
            {
                Animation* anim = new Animation();
                if(!anim->loadFromXMLNode(*node, mFrames))
                {
                    releaseAnimations();
                    releaseFrames();
                    releaseTextures();
                    return false;
                }
                //warn about empty animations, don't add them
                if(anim->isEmpty())
                {
                    sf::err() << "Warning: Empty animation \"" << anim->getName() << " in Spriter file!" << std::endl;
                    delete anim;
                    continue;
                }
                mAnimations[anim->getName()] = anim;
            }
            //even if there were no animations we succeeded in loading nothing.
            if(mAnimations.empty())
            {
                //let's warn about this - it's probably unintended.
                sf::err() << "Warning: No animations in Spriter file!" << std::endl;
                releaseFrames();
                releaseTextures();
                return true;
            }
            //set current animation to first one available
            mCurrentAnimation = mAnimations.begin();
            //load bounding box
            rapidxml::xml_node<char>* bboxNode = characterNode->first_node("box");
            if(!bboxNode)
            {
                sf::err() << "Warning: Character contains no bounding box information!" << std::endl;
            }
            else
            {
                rapidxml::xml_node<char>* bottomNode = bboxNode->first_node("bottom"),
                    *topNode = bboxNode->first_node("top"),
                    *leftNode = bboxNode->first_node("left"),
                    *rightNode = bboxNode->first_node("right");
                if(!bottomNode || !topNode || !leftNode || !rightNode)
                {
                    sf::err() << "Error: Invalid Spriter file: Character's bounding box information incomplete!" << std::endl;
                    return false;
                }
                float bottom = atof(bottomNode->value());
                float top = atof(topNode->value());
                float right = atof(rightNode->value());
                float left = atof(leftNode->value());
                mBoundingBox.left = left;
                mBoundingBox.top = top;
                mBoundingBox.width = right - left;
                mBoundingBox.height = bottom - top;
            }

        }
        catch(const rapidxml::parse_error& err)
        {
            sf::err() << "Error parsing XML file: " << err.what();
            return false;
        }
        return true;
    }

    sf::Texture* Character::getTexture(const std::string& filename)
    {
        //has this texture already been loaded?
        TextureMap::iterator pos = mTextures.find(filename);
        if(pos != mTextures.end())
        {
            //yes. use previously loaded instance.
            return pos->second;
        }
        //not loaded yes. try loading it.
        sf::Texture* tex = mTextureManager.getTexture(filename);
        //success or not, cache it.
        mTextures[filename] = tex;
        return tex;
    }

    void Character::releaseTextures()
    {
        for(TextureMap::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
        {
            mTextureManager.releaseTexture(it->second);
        }
        mTextures.clear();
    }

    void Character::releaseFrames()
    {
        for(FrameMap::iterator it = mFrames.begin(); it != mFrames.end(); ++it)
        {
            delete it->second;
        }
        mFrames.clear();
    }

    void Character::releaseAnimations()
    {
        for(AnimationMap::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
        {
            delete it->second;
        }
        mAnimations.clear();
        mCurrentAnimation = mAnimations.end();
    }

    void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        if(mCurrentAnimation != mAnimations.end())
        {
            target.draw(*(mCurrentAnimation->second), states);
        }
    }

    const bool Character::setAnimation(const std::string& animationName)
    {
        AnimationMap::iterator anim = mAnimations.find(animationName);
        if(anim != mAnimations.end())
        {
            anim->second->restart();
            mCurrentAnimation = anim;
            return true;
        }
        return false;
    }

    void Character::update(const sf::Uint64 deltaT_ms)
    {
        if(mCurrentAnimation != mAnimations.end())
        {
            mCurrentAnimation->second->update(deltaT_ms);
        }
    }
}
