#include <iostream>
#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/FPSCounter.hpp"
#include <vector>

Application::Application()
{
    window = sf::RenderWindow(sf::VideoMode({640 * 2, 360 * 2}), "CMake SFML Project");
    window.setVerticalSyncEnabled(true);

    unsigned int tmp_x = 640;
    unsigned int tmp_y = 360;
    /*
        For future reference, making something like this below
        is like opening tequila on sunday night
    */
    //for testing only
    c_ptr = std::make_shared<sf::CircleShape>(tmp_x / 5);
    c_ptr->setFillColor(sf::Color::Green);
    c_ptr->setOutlineThickness(tmp_x/5/15);
    c_ptr->setOutlineColor(sf::Color::Blue);
    c_ptr->setOrigin({c_ptr->getRadius(), c_ptr->getRadius()});
    c_ptr->setPosition({tmp_x / 2.f, tmp_y / 2.f});

    root = std::make_shared<FPSCounter>();
    std::weak_ptr<FPSCounter> fps = std::dynamic_pointer_cast<FPSCounter>(root);
    fps.lock()->set_position({15, 15});
    
    //for testing only, will be moved to a proper function later

    /*
    //addition test - passed
    mg.set_window(&window);
    mg.add_layer("Test layer", 0, {tmp_x, tmp_y});
    mg.add_drawable("Test layer", std::weak_ptr(c_ptr));
    mg.add_layer("UI", 4, {1920u, 1240u});  //rendered after
    mg.add_drawable("UI", std::weak_ptr<sf::Text>(fps.lock()->text));
    mg.move_view("UI", {1800, 500}); //viewport moves confirmed
    */

    //stess testing
    mg.set_window(&window);
    mg.add_layer("0", 0, {1920u, 1240u});
    mg.add_layer("UI", 255, {1920u, 1240u});
    mg.add_drawable("UI", std::weak_ptr<sf::Text>(fps.lock()->text));
}

Application& Application::instance()
{
    static Application instance_;
    return instance_;
}

void Application::run()
{
    static std::vector<std::shared_ptr<sf::CircleShape>> circles;
    circles.reserve(256 * 501);
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
            if(event->is<sf::Event::Closed>())
            {
                std::cerr << circles.size();
                this->close();
            }
            else if(auto e = event->getIf<sf::Event::Resized>())
            {
                mg.rescale();
            }
        }

        float delta = clock.restart().asSeconds();
        if(root) root->update(delta);
    
        //testing enviroment below    
        mg.update(delta);
        //***********************************************
        
        for(int i = 0; i < 10; ++i)
        {
            std::shared_ptr<sf::CircleShape> c = std::make_shared<sf::CircleShape>(15.f);
            c->setFillColor(sf::Color::Green);
            c->setPosition({rand()%1920, rand()%1240});
            circles.push_back(c);
            mg.add_drawable("0", std::weak_ptr(c));
        }
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
