#include <SFML/Graphics.hpp>

int main()
{
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
