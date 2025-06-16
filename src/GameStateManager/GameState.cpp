#include "GameStateManager/GameState.hpp"
#include "GameStateManager/GameStateManager.hpp"
#include "DungeonManager/DungeonManager.hpp"
#include "Events/WindowEventManager.hpp"
#include "Nodes/Application.hpp"
#include <fstream>
#include <filesystem>
#include <random>

constexpr const char layer[] = "Debug_ui";

inline std::string get_path()
{
    #ifdef DEBUG
    std::filesystem::create_directories("build/runtime_files");
    return "build/runtime_files/save.txt";
    #else
    return "save.txt";
    #endif
}

void MainMenuState::initialize()
{
    auto title = add_child<Label>(layer, Application::instance().font, "The Game", 100).lock();
    title->setPosition({20.f, 20.f});
    auto menu_ptr = add_child<Menu>().lock();
    menu_ptr->add_item(SelectableLabel([](){
        Application::instance().get_manager<GameStateManager>()->change_state<GameSetupState>();
    }, layer, Application::instance().font, "Play", 40));

    menu_ptr->add_item(SelectableLabel([](){

        std::ifstream file(get_path());

        if(!file.is_open())
        {
            return;
        }
        
        std::size_t seed;
        sf::Vector2u dungeon_size;
        unsigned int chunk_size;
        sf::Vector2i player_pos_d;
        sf::Vector2i player_pos_c;

        file >> seed >> dungeon_size.x >> dungeon_size.y >> chunk_size >>
            player_pos_d.x >> player_pos_d.y >> player_pos_c.x >> player_pos_c.y;

        Application::instance().get_manager<GameStateManager>()->
            change_state<GameState>(seed, dungeon_size, chunk_size, player_pos_d, player_pos_c);
        file.close();
    }, layer, Application::instance().font, "Load", 40));

    menu_ptr->add_item(SelectableLabel([](){
        Application::instance().close();
    }, layer, Application::instance().font, "Quit", 40));

    menu_ptr->setPosition(title->getPosition() + sf::Vector2f{0.f, title->getGlobalBounds().size.y + 50.f});
}

void MainMenuState::update(float delta) {}

GameSetupState::GameSetupState()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist;
    seed = dist(gen);
}

void GameSetupState::initialize()
{
    auto title = add_child<Label>(layer, Application::instance().font, "The Game", 100).lock();
    title->setPosition({20.f, 20.f});
    menu_ptr = add_child<Menu>().lock();

    seed_l = menu_ptr->add_item(SelectableLabel([&](){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist;
        seed = dist(gen);
    }, layer, Application::instance().font, "Seed: " + std::to_string(seed), 60));

    d_size_l = menu_ptr->add_item(SelectableLabel([&](){
        ++selected_d_size;
        if(selected_d_size >= dungeon_sizes.size())
        {
            selected_d_size = 0;
        }
    }, layer, Application::instance().font, "Dungeon size: " + std::to_string(dungeon_sizes[selected_d_size]), 60));

    c_size_l = menu_ptr->add_item(SelectableLabel([&](){
        ++selected_c_size;
        if(selected_c_size >= chunk_sizes.size())
        {
            selected_c_size = 0;
        }
    }, layer, Application::instance().font, "Chunk_size: " + std::to_string(chunk_sizes[selected_c_size]), 60));

    menu_ptr->add_item(SelectableLabel([&](){
        Application::instance().get_manager<GameStateManager>()->change_state<GameState>(
            seed, 
            sf::Vector2u{dungeon_sizes[selected_d_size], dungeon_sizes[selected_d_size]},
            chunk_sizes[selected_c_size]
        );
    }, layer, Application::instance().font, "Approve", 60));

    menu_ptr->add_item(SelectableLabel([&](){
        Application::instance().get_manager<GameStateManager>()->change_state<MainMenuState>();
    }, layer, Application::instance().font, "Return", 60));


    menu_ptr->setPosition(title->getPosition() + sf::Vector2f{0.f, title->getGlobalBounds().size.y + 50.f});
}

void GameSetupState::update(float delta)
{
    if(auto ptr = seed_l.lock())
    {
        ptr->setString("Seed: " + std::to_string(seed));
    }
    if(auto ptr = d_size_l.lock())
    {
        ptr->setString("Dungeon size: " + std::to_string(dungeon_sizes[selected_d_size]));
    }
    if(auto ptr = c_size_l.lock())
    {
        ptr->setString("Chunk_size: " + std::to_string(chunk_sizes[selected_c_size]));
    }
}

GameState::GameState(std::size_t seed, sf::Vector2u dungeon_size, unsigned int chunk_size,
                    sf::Vector2i player_pos_d, sf::Vector2i player_pos_c)
{
    Application::instance().
        register_manager<DungeonManager>
        (Application::instance().atlas, seed, dungeon_size, chunk_size, player_pos_d, player_pos_c);
}

void GameState::initialize()
{
    ev_id = Application::instance().get_manager<WindowEventManager>()->get_event<sf::Event::KeyPressed>()->
        subscribe(shared_from_this(), [&](const sf::Event::KeyPressed& e)
        {
            if(e.scancode == sf::Keyboard::Scancode::Enter)
            {
                Application::instance().get_manager<DungeonManager>()->move({0, 0});
            }
            else if(e.scancode == sf::Keyboard::Scancode::Up)
            {
                Application::instance().get_manager<DungeonManager>()->move({0, 1});
            }
            else if(e.scancode == sf::Keyboard::Scancode::Down)
            {
                Application::instance().get_manager<DungeonManager>()->move({0, -1});
            }
            else if(e.scancode == sf::Keyboard::Scancode::Right)
            {
                Application::instance().get_manager<DungeonManager>()->move({-1, 0});
            }
            else if(e.scancode == sf::Keyboard::Scancode::Left)
            {
                Application::instance().get_manager<DungeonManager>()->move({1, 0});
            }
            else if(e.scancode == sf::Keyboard::Scancode::Escape)
            {
                if(pause_menu.lock() == nullptr)
                {
                    Application::instance().get_manager<DungeonManager>()->block_movement = true;
                    pause_text = add_child<Label>(
                        layer,
                        Application::instance().font,
                        "Paused",
                        120
                    );
                    pause_text.lock()->setPosition({
                        Application::instance().get_window().getSize().x / 2.f,
                        200.f
                    });

                    pause_menu = add_child<Menu>();
                    pause_menu.lock()->add_item(SelectableLabel(
                        [&](){
                            Application::instance().get_manager<DungeonManager>()->block_movement = false;
                            if(auto ptr = pause_menu.lock())
                            {
                                ptr->kill();
                            }
                            if(auto ptr = pause_text.lock())
                            {
                                ptr->kill();
                            }
                        },
                        layer,
                        Application::instance().font,
                        "Resume",
                        60
                    ));
                    pause_menu.lock()->add_item(SelectableLabel(
                        [&](){
                            Application::instance().get_manager<DungeonManager>()->block_movement = false;
                            if(auto ptr = pause_menu.lock())
                            {
                                ptr->kill();
                            }
                            if(auto ptr = pause_text.lock())
                            {
                                ptr->kill();
                            }

                            std::ofstream file(get_path());

                            file << Application::instance().get_manager<DungeonManager>()->as_string();
                            file.close();
                        },
                        layer,
                        Application::instance().font,
                        "Save",
                        60
                    ));
                    pause_menu.lock()->add_item(SelectableLabel(
                        [&](){
                            Application::instance().get_manager<DungeonManager>()->block_movement = false;
                            if(auto ptr = pause_menu.lock())
                            {
                                ptr->kill();
                            }
                            if(auto ptr = pause_text.lock())
                            {
                                ptr->kill();
                            }

                            std::ofstream file(get_path());

                            file << Application::instance().get_manager<DungeonManager>()->as_string();
                            file.close();

                            Application::instance().get_manager<GameStateManager>()->change_state<MainMenuState>();
                        },
                        layer,
                        Application::instance().font,
                        "Save and quit to title",
                        60
                    ));

                    pause_menu.lock()->setPosition({
                        pause_text.lock()->getPosition().x,
                        pause_text.lock()->getPosition().y + pause_text.lock()->getGlobalBounds().size.y + 50.f
                    });
                }
                else
                {
                    Application::instance().get_manager<DungeonManager>()->block_movement = false;
                    if(auto ptr = pause_text.lock())
                    {
                        ptr->kill();
                    }
                    if(auto ptr = pause_menu.lock())
                    {
                        ptr->kill();
                    }
                }
            }
    });
}

void GameState::update(float delta)
{

}
GameState::~GameState()
{
    if(Application::instance().get_window().isOpen())
    {
        Application::instance().deregister_manager<DungeonManager>();
    }
}