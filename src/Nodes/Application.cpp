#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/CollisionManager.hpp"
#include "Nodes/FPSCounter.hpp"
#include "Tilemap/Tilemap.hpp"
#include "Events.hpp"
#include <stack>
#include <array>

//debug
#include "Nodes/MouseCollider.hpp"
#include "Nodes/Icon.hpp"

std::mutex Application::application_mutex;

void Application::initialize()
{
    /*
        If you want to have a bad time add a node here, i dare you
    */
    window = sf::RenderWindow(sf::VideoMode({640 * 2, 360 * 2}), "CMake SFML Project");
    window.setVerticalSyncEnabled(true);

    /*
     This block can stay for now, but this should be handled properly later on
    (program argumetns -d as an separete debug node maybe?)
    std::shared_ptr<FPSCounter> fps = std::make_shared<FPSCounter>();
    fps->set_position({15, 15});
    root_level.push_back(fps);
    */

    static TextureAtlas atlas("build/runtime_files/Dungeon_Tileset.png");
    static std::shared_ptr<Tilemap> tilemap = std::make_shared<Tilemap>();
    constexpr std::array<sf::Vector2i, 30> tiles = {
        sf::Vector2i(0, 0), sf::Vector2i(1, 0), sf::Vector2i(2, 0), sf::Vector2i(3, 0), sf::Vector2i(4, 0), sf::Vector2i(5, 0),
        sf::Vector2i(0, 1), sf::Vector2i(1, 1), sf::Vector2i(2, 1), sf::Vector2i(3, 1), sf::Vector2i(4, 1), sf::Vector2i(5, 1),
        sf::Vector2i(0, 2), sf::Vector2i(1, 2), sf::Vector2i(2, 2), sf::Vector2i(3, 2), sf::Vector2i(4, 2), sf::Vector2i(5, 2),
        sf::Vector2i(0, 3), sf::Vector2i(1, 3), sf::Vector2i(2, 3), sf::Vector2i(3, 3), sf::Vector2i(4, 3), sf::Vector2i(5, 3),
        sf::Vector2i(0, 4), sf::Vector2i(1, 4), sf::Vector2i(2, 4), sf::Vector2i(3, 4), sf::Vector2i(4, 4), sf::Vector2i(5, 4)
    };
    tilemap->load(atlas, {16, 16}, tiles.data(), 6, 5);
    tilemap->setPosition({1920.f / 6.f, 1240.f / 6.f});
    tilemap->setScale({5.f, 5.f});


    register_manager<RenderManager>();          //maybe should be added first
    register_manager<WindowEventManager>();     //just an empty node, at least for now
    register_manager<CollisionManager>();

    get_manager<RenderManager>()->add_layer("Debug_ui", 250, {1920u, 1240u});
    //priority is 250 so any popup window (e.g. pause menu) will go on top of the debug info
    get_manager<RenderManager>()->add_drawable("Debug_ui", tilemap);

    get_manager<CollisionManager>()->add_layer("Debug_coll", 0);

    //for testing collisions
    root_level.push_back(create<DebugRect>());
    //root_level.push_back(create<DebugCirc>());
    root_level.push_back(create<MouseCursor>("Debug_ui", "Debug_coll"));
    root_level.push_back(create<FPSCounter>());

    auto ico = create<Icon>("Debug_ui", atlas.get_texture(), sf::IntRect{{0, 0}, {32, 32}});
    ico->set_scale({5.f, 5.f});
    ico->set_position({1600.f, 0.f});
    root_level.push_back(ico);

    get_manager<WindowEventManager>()->get_event<sf::Event::Closed>()->
        subscribe([&](const sf::Event::Closed& e){close();});
    //get_manager<WindowEventManager>()->get_event<sf::Event::Resized>()->
        //subscribe([&](const sf::Event::Resized& e){get_manager<RenderManager>()->rescale();});

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
    //this is in case someone treis to create Application in two
    //separate threads at once, which we will not be doing
    //as threads are scary
    std::lock_guard<std::mutex> lock(application_mutex);
    static Application instance_;
    return instance_;
}

void Application::run()
{
    /*
        What should happen here:
         - no setting up anything, just the loop (leave setup in constructor)
            {This might not be a good idea after all, maybe some one time setup function?}
         - start whiel loop
         - process events
         - update each tree node (bfs alghorithm)
         - draw and display
    */

    this->initialize();

    while (window.isOpen())
    {
        float delta = clock.restart().asSeconds();

        struct StackElement //helper for dfs tree search
        {
        std::shared_ptr<Node> ptr;
        bool visited;
        };

        std::stack<StackElement> s;   //pointer : visited pair

        /*
            DFS for three travelsal, updates children first, then the parent
            Depending on SceneManager implementation that may be moved there
        */

        for(auto& node : root_level)
        {
            s.push({node, false});
        }
            
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

const std::vector<std::shared_ptr<Node>>& Application::get_root_level()
{
    return root_level;
}

void Application::close()
{
    #ifdef DEBUG
    Logger::log(Logger::MessageType::Info, print_tree());
    #endif
    //acts pretty much like a destructor, so any autosave on quit goes here");
    window.close();
}

sf::RenderWindow& Application::get_window()
{
    return this->window;
}
