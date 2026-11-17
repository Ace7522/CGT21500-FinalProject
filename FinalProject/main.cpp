#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "Core/Engine.h"

using namespace sf;
using namespace sfp;

int main()
{
    // Initialize window and world
    RenderWindow window(
        VideoMode(1556, 2048), 
        "Final Project: Space Invaders", 
        Style::Titlebar | Style::Close
    );
    World world(Vector2f(0, 0));

    // Initialize the clock
    Clock clock;
    Time lastTime(clock.getElapsedTime());
    Time currentTime(lastTime);

    // Initialize the game
    Engine engine(window, world);

    // Render while the window is open
    while (window.isOpen())
    {
        // Detect if the window was closed
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        // Get elapsed time
        currentTime = clock.getElapsedTime();
        Time deltaTime = currentTime - lastTime;
        long deltaMS = deltaTime.asMilliseconds();

        // Frame rate
        if (deltaMS > 9)
        {
            lastTime = currentTime;
            
            engine.update(deltaMS);
            engine.render();
        }
    }
}