#include "GameStateManager/GameStates.hpp"
#include "Events/WindowEventManager.hpp"
#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "GameStateManager/GameStateManager.hpp"
#include "DungeonManager/DungeonManager.hpp"
#include "Nodes/ActorManager.hpp"
#include <fstream>
#include <filesystem>
#include <random>

constexpr const char layer[] = "Debug_ui";
constexpr unsigned int text_size = 60;

inline std::string get_path()
{
    #ifdef DEBUG
    std::filesystem::create_directories("build/runtime_files");
    return "build/runtime_files/save.txt";
    #else
    return "save.txt";
    #endif
}

inline void center(std::shared_ptr<Label> tf, float v_offset)
{
    auto layer_tex_size = Application::instance().get_manager<RenderManager>()->get_render_texture(layer).getSize();
    tf->setOrigin({tf->getGlobalBounds().size.x / 2.f, 0.f});
    tf->setPosition({layer_tex_size.x / 2.f, v_offset});
}

GameState::GameState(std::size_t seed, sf::Vector2u dungeon_size, unsigned int chunk_size,
                    sf::Vector2i player_pos_d, sf::Vector2i player_pos_c)
{
    Application::instance().register_manager<DungeonManager>(
        Application::instance().atlas,
        seed,
        dungeon_size,
        chunk_size,
        player_pos_d,
        player_pos_c
    );
    
    Application::instance().register_manager<ActorManager>();
}

void GameState::initialize()
{
    ev_id = Application::instance().get_manager<WindowEventManager>()->get_event<sf::Event::KeyReleased>()->
        subscribe(shared_from_this(), [&](const sf::Event::KeyReleased& e)
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
                    auto layer_sp = Application::instance().get_manager<RenderManager>()->get_render_sprite(layer);
                    Application::instance().get_manager<DungeonManager>()->block_movement = true;
                    pause_text = add_child<Label>(
                        layer,
                        Application::instance().font,
                        "Paused",
                        text_size * 2
                    );
                    center(pause_text.lock(), 200.f);

                    pause_menu = add_child<Menu>();
                    pause_menu.lock()->setPosition({
                        pause_text.lock()->getPosition().x,
                        pause_text.lock()->getPosition().y + pause_text.lock()->getGlobalBounds().size.y + 50.f
                    });
                    pause_menu.lock()->set_alignment(Menu::Style::align_center);

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
                        text_size
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
                        text_size
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
                        text_size
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

                            Application::instance().get_manager<GameStateManager>()->change_state<MainMenuState>();
                        },
                        layer,
                        Application::instance().font,
                        "Quit to title",
                        text_size
                    ));
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

    auto& atlas = Application::instance().atlas;
    std::vector<Actor> partyVec{
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{0, 8}, "Debug_ui"},
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{0, 8}, "Debug_ui"},
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{0, 8}, "Debug_ui"},
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{0, 8}, "Debug_ui"},
        {"Name", ActorRaceEnum::Lich, std::make_shared<DummyBehaviour>(), atlas, sf::Vector2i{0, 8}, "Debug_ui"}
    };
    auto p = Application::instance().get_manager<ActorManager>()->add_party(partyVec);

    p->display({300.f, 340.f}, {5.f, 5.f}, {10.f, 10.f}, true);
    p->display_cards();
}

void GameState::update(float delta)
{

}
GameState::~GameState()
{
    if(Application::instance().get_window().isOpen())
    {
        Application::instance().deregister_manager<DungeonManager>();
        Application::instance().deregister_manager<ActorManager>();
    }
}