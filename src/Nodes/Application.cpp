#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/FPSCounter.hpp"

Application::Application()
{
    window = sf::RenderWindow(sf::VideoMode({640 * 2, 360 * 2}), "CMake SFML Project");
    window.setVerticalSyncEnabled(true);

    //This block can stay for now, but this should be handled properly later on (program argumetns -d maybe?)
    root = std::make_shared<FPSCounter>();
    std::weak_ptr<FPSCounter> fps = std::dynamic_pointer_cast<FPSCounter>(root);
    fps.lock()->set_position({15, 15});

    register_manager<RenderManager>();

    get_manager<RenderManager>()->set_window(&window);
    get_manager<RenderManager>()->add_layer("UI", 255, {1920u, 1240u});
    get_manager<RenderManager>()->add_drawable("UI", std::weak_ptr<sf::Text>(fps.lock()->text));
    //********************************************/
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
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                this->close();
            }
            else if(auto e = event->getIf<sf::Event::Resized>())
            {
                get_manager<RenderManager>()->rescale();   //for proper rendering
            }
        }

        float delta = clock.restart().asSeconds();
        if(root) root->update(delta);
    
        //this should be the first element added to the queue in bfs 
        get_manager<RenderManager>()->update(delta);   //handle rendering
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
