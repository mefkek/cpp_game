#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/FPSCounter.hpp"
#include <stack>

sf::Vector2f pos = {0.f , 0.f};
class TestNode : public Node
{
    private:
    std::shared_ptr<sf::CircleShape> c;
    float timeout = 5.f;
    int time_steps = 4.5f;
    
    public:
    TestNode()
    {
        c = std::make_shared<sf::CircleShape>(25.f);
        c->setPosition(pos);
        Application::instance().get_manager<RenderManager>()->add_drawable("UI", c);
        pos += {25.f, 25.f};
    };

    void update(float delta) override
    {
        timeout -= delta;
        if(timeout <= 0.f)
        {
            kill();  
        }
        else if(timeout <= time_steps)
        {
            this->add_child<TestNode>();
            time_steps -= 0.5f;
        }
    }
};


Application::Application()
{
    /*
        If you want to have a bad time add a node here, i dare you
    */
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
    root->add_child<TestNode>();
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
        int child_count = 0;

        if(root)
        {
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
                    child_count++;
                    s.pop();
                }
            }
        }
        
        std::cerr << child_count << '\n';
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
