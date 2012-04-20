#include <SFML/Graphics.hpp>
#include <SFMLSpriter/Character.hpp>
#include <SFMLSpriter/SimpleTextureManager.hpp>

int main()
{
    SFMLSpriter::SimpleTextureManager textureManager; //C++ guarantees deletion in reverse order of creation, so the textureManager will still live when testChar dies and thus releases its textures.
    SFMLSpriter::Character testChar(textureManager);
    sf::RenderWindow wnd(sf::VideoMode(800, 600), "SFML Spriter Test");
    while(wnd.isOpen())
    {
        // Drawing (before logic so it can be done in parallel)
        wnd.clear();

        // Event Loop
        sf::Event ev;
        while(wnd.pollEvent(ev))
        {
            if(ev.type == sf::Event::Closed)
            {
                wnd.close();
            }
        }

        // Swap buffers
        wnd.display();
    }
    return 0;
}
