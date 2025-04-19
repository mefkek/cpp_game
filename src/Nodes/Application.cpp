#include <iostream>
#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"

Application::Application()
{
    window = sf::RenderWindow(sf::VideoMode({640 * 2, 360 * 2}), "CMake SFML Project");
    window.setFramerateLimit(144);

    //for testing only
    c_ptr = std::make_shared<sf::CircleShape>(75.f);
    c_ptr->setFillColor(sf::Color::Green);
    c_ptr->setOutlineThickness(-5);
    c_ptr->setOutlineColor(sf::Color::Blue);
    c_ptr->setOrigin({c_ptr->getRadius(), c_ptr->getRadius()});
    c_ptr->setPosition({640/2.f, 360/2.f});
    
    //for testing only will be mvoed to a proper function later
    mg.set_window(&window);
    mg.add_layer("Test layer");
    mg.add_drawable("Test layer", std::weak_ptr(c_ptr));
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

    //removed temporarly, will remove permanently after menagers are implemented
    /*
    sf::CircleShape c(75);
    c.setFillColor(sf::Color::Green);
    c.setOutlineThickness(-5);
    c.setOutlineColor(sf::Color::Blue);
    c.setOrigin({75.f, 75.f});
    c.setPosition({600/2.f, 450/2.f});
    */
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                this->close();
            }
        }

        float delta = clock.restart().asSeconds();
        if(root) root->update(delta);
    
        //testing enviroment below    
        mg.update(delta);
        //***********************************************

    }
}

void Application::close()
{
    //acts pretty much like a destructor, so any autosave on quit goes here
    window.close();
}

sf::RenderWindow& Application::get_window()
{
    return this->window;
}
