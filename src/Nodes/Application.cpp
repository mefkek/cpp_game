#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "Nodes/CollisionManager.hpp"
#include "DungeonManager/DungeonManager.hpp"
#include "Nodes/ActorManager.hpp"
#include "../../include/Ui/FPSCounter.hpp"
#include "Events.hpp"
#include <stack>
#include <array>

//debug
#include "../../include/Ui/MouseCollider.hpp"
#include "Ui/Icon.hpp"
#include "../../include/Ui/Button.hpp"

std::mutex Application::application_mutex;

Application::Application() : font("Fonts/ARIAL.TTF"), atlas("Textures/Tileset.png") {}

void Application::initialize()
{
    window = sf::RenderWindow(sf::VideoMode({640 * 2, 360 * 2}), "CMake SFML Project");
    window.setVerticalSyncEnabled(true);

    register_manager<RenderManager>();
    register_manager<WindowEventManager>(); 
    register_manager<CollisionManager>();
    register_manager<DungeonManager>(atlas, 10, sf::Vector2u{255u, 255u}, 32);
    register_manager<ActorManager>();

    get_manager<RenderManager>()->add_layer("Debug_ui", 250, {1920u, 1240u});

    get_manager<CollisionManager>()->add_layer("Debug_coll_trig", 0);
    get_manager<CollisionManager>()->add_layer("Debug_coll_coll", 0);

    root_level.push_back(create<MouseCursor>("Debug_ui", "Debug_coll_coll"));
    root_level.push_back(create<FPSCounter>("Debug_ui", font));

    get_manager<WindowEventManager>()->get_event<sf::Event::Closed>()->
        subscribe([&](const sf::Event::Closed& e){close();});

    get_manager<WindowEventManager>()->get_event<sf::Event::KeyPressed>()->
        subscribe([&](const sf::Event::KeyPressed& e)
        {
            if(e.scancode == sf::Keyboard::Scancode::Enter)
            {
                get_manager<DungeonManager>()->move({0, 0});
            }
            else if(e.scancode == sf::Keyboard::Scancode::Up)
            {
                get_manager<DungeonManager>()->move({0, 1});
            }
            else if(e.scancode == sf::Keyboard::Scancode::Down)
            {
                get_manager<DungeonManager>()->move({0, -1});
            }
            else if(e.scancode == sf::Keyboard::Scancode::Right)
            {
                get_manager<DungeonManager>()->move({-1, 0});
            }
            else if(e.scancode == sf::Keyboard::Scancode::Left)
            {
                get_manager<DungeonManager>()->move({1, 0});
            }
        });

    std::vector<Actor> partyVec{
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{0, 8}, "Debug_ui"},
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{0, 8}, "Debug_ui"},
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{0, 8}, "Debug_ui"},
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{0, 8}, "Debug_ui"},
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{0, 8}, "Debug_ui"}
    };
    auto p = get_manager<ActorManager>()->addParty(partyVec);

    std::vector<Actor> enemy_party_vec{
        //{ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{1, 8}, "Debug_ui"},
        //{ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{1, 8}, "Debug_ui"},
        //{ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{1, 8}, "Debug_ui"},
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{1, 8}, "Debug_ui"},
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{1, 8}, "Debug_ui"}
    };
    auto ep = get_manager<ActorManager>()->addParty(enemy_party_vec);
    //********************************************/
    p->display({300.f, 340.f}, {5.f, 5.f}, {10.f, 10.f}, true);
    p->display_cards();
    ep->display({300.f + 750.f, 340.f}, {-5.f, 5.f}, {-10.f, 10.f});
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
