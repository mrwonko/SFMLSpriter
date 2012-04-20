#include <SFMLSpriter/Character.hpp>
#include <SFML/System/Err.hpp>
#include <rapidxml.hpp>
#include <fstream>
#include <streambuf>

namespace SFMLSpriter
{
    Character::Character(TextureManager& textureManager) :
        mTextureManager(textureManager)
    {
        //ctor
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

    const bool Character::loadFromMemory(const std::string& xml, std::string rootDir)
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
        //TODO - Actual loading
        return false;
    }

    void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        //TODO - Drawing
    }
}
