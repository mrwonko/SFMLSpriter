#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFMLSpriter/Character.hpp>
#include <SFMLSpriter/SimpleTextureManager.hpp>

#include <cassert>

// Animation Example Application
//
// Animations are cycled using left/right

int main()
{
    //animation names
    static const unsigned int NUM_ANIMATIONS = 2;
    static const char* animations[NUM_ANIMATIONS] =
    {
        "idle_healthy",
        "walk"
    };
    unsigned int currentAnimation = 0;

    //Create a texture manager that is used to load the images
    //C++ guarantees deletion in reverse order of creation, so the textureManager will still live when testChar dies and can thus releases its textures.
    SFMLSpriter::SimpleTextureManager textureManager;
    //load the Spriter file
    SFMLSpriter::Character testChar(textureManager);
    if(!testChar.loadFromFile("example hero/BetaFormatHero.SCML"))
    {
        return 0;
    }
    //move it to the screen's center
    testChar.setPosition(400, 300);
    //bounding box visualization
    sf::RectangleShape bbox(sf::Vector2f(testChar.getBoundingBox().width, testChar.getBoundingBox().height));
    bbox.setPosition(400.f + testChar.getBoundingBox().left, 300.f + testChar.getBoundingBox().top);
    bbox.setFillColor(sf::Color::Transparent);
    bbox.setOutlineColor(sf::Color::White);
    bbox.setOutlineThickness(1);

    //Create window
    sf::RenderWindow wnd(sf::VideoMode(800, 600), "SFML Spriter Test");
    //Create timer to get frame time
    sf::Clock timer;
    sf::Uint64 lastFrametime = 0;
    while(wnd.isOpen())
    {
        // Drawing (before logic so it can be done in parallel)
        wnd.clear();
        wnd.draw(testChar);
        wnd.draw(bbox);

        // Event Loop
        sf::Event ev;
        while(wnd.pollEvent(ev))
        {
            if(ev.type == sf::Event::Closed)
            {
                wnd.close();
            }
            if(ev.type == sf::Event::KeyPressed)
            {
                switch(ev.key.code)
                {
                case sf::Keyboard::Escape:
                    wnd.close();
                    break;
                case sf::Keyboard::Left:
                    if(currentAnimation == 0) currentAnimation = NUM_ANIMATIONS - 1;
                    else currentAnimation -= 1;
                    testChar.setAnimation(animations[currentAnimation]);
                    break;
                case sf::Keyboard::Right:
                    currentAnimation += 1;
                    if(currentAnimation >= NUM_ANIMATIONS) currentAnimation = 0;
                    testChar.setAnimation(animations[currentAnimation]);
                    break;
                default:
                    break;
                }
            }
        }

        sf::Uint64 frametime = timer.getElapsedTime().asMilliseconds();
        sf::Uint64 deltaT_ms = frametime - lastFrametime;
        lastFrametime = frametime;
        testChar.update(deltaT_ms);

        // Swap buffers
        wnd.display();
    }
    return 0;
}
