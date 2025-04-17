#include "../include/Application.hpp"

Application::Application()
{
    window = sf::RenderWindow(sf::VideoMode({600u, 450u}), "CMake SFML Project");
    window.setFramerateLimit(144);
}

Application& Application::instance()
{
    static Application instance_;
    return instance_;
}

void Application::run()
{
    /*
        What should happen here:
         - no setting up anything, just the loop (leave setup in constructor)
         - start whiel loop
         - process events
         - update each tree node (bfs alghorithm)
         - draw and display
    */
    sf::CircleShape c(75);
    c.setFillColor(sf::Color::Green);
    c.setOutlineThickness(-5);
    c.setOutlineColor(sf::Color::Blue);
    c.setOrigin({75.f, 75.f});
    c.setPosition({600/2.f, 450/2.f});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                this->close();
            }
        }

        window.clear();
        window.draw(c);
        window.display();
    }
    
    //yeet above out later
    float delta = clock.restart().asSeconds();
    if(root) root->update(delta);
}

void Application::close()
{
    //acts pretty much like a destructor, so any autosave on quit goes here
    window.close();
}
