#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/FPSCounter.hpp"
#include <stack>

std::mutex Application::application_mutex;

Application::Application()
{
    /*
        If you want to have a bad time add a node here, i dare you
    */
    window = sf::RenderWindow(sf::VideoMode({640 * 2, 360 * 2}), "CMake SFML Project");
    window.setVerticalSyncEnabled(true);

    //This block can stay for now, but this should be handled properly later on
    //(program argumetns -d as an separete debug node maybe?)
    root = std::make_shared<FPSCounter>();
    std::weak_ptr<FPSCounter> fps = std::dynamic_pointer_cast<FPSCounter>(root);
    fps.lock()->set_position({15, 15});

    register_manager<RenderManager>();
    register_manager<CollisionManager>();

    get_manager<RenderManager>()->set_window(&window);
    get_manager<RenderManager>()->add_layer("Debug_ui", 250, {1920u, 1240u});
    //priority is 250 so any popup window (e.g. pause menu) will go on top of the debug info
    get_manager<RenderManager>()->add_drawable("Debug_ui", std::weak_ptr<sf::Text>(fps.lock()->text));

    //uncomment for testing
    /*
    get_manager<CollisionManager>()->add_layer("Test Layer", 1);
    get_manager<CollisionManager>()->add_collider("Test Layer", insert rectangle collider);
    get_manager<CollisionManager>()->add_collider("Test Layer", insert cirlce collider, preferably following mouse);
    get_manager<CollisionManager>()->add_collider("Test Layer", insert cirlce collider);
    /*
    //********************************************/
}

Application& Application::instance()
{
    std::lock_guard<std::mutex> lock(application_mutex);
    static Application instance_;
    return instance_;
}

struct StackElement //helper for dfs tree search
{
    std::shared_ptr<Node> ptr;
    bool visited;
};

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
        std::stack<StackElement> s;   //pointer : visited pair

        //managers should be update in the node that added them

        /*
            DFS for three travelsal, updates children first, then the parent
            Depending on SceneManager implementation that may be moved there
        */

        s.push({get_manager<RenderManager>(), false});
        s.push({get_manager<CollisionManager>(), false});
        s.push({root, false});
            
        while(!s.empty())
        {
            StackElement& top = s.top();

            if(!top.visited)
            {
                top.visited = true;
                for(auto it = top.ptr->get_children().rbegin(); it != top.ptr->get_children().rend(); ++it)
                {
                    s.push({*it, false});
                }
            }
            else
            {
                top.ptr->update(delta);
                s.pop();
            }
        }
    }
}

void Application::close()
{
    //acts pretty much like a destructor, so any autosave on quit goes here");
    window.close();
}

sf::RenderWindow& Application::get_window()
{
    return this->window;
}
