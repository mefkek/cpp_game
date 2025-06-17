#include "GameStateManager/GameStates.hpp"
#include "Events/WindowEventManager.hpp"
#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "GameStateManager/GameStateManager.hpp"
#include "DungeonManager/DungeonManager.hpp"
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

GameSetupState::GameSetupState()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist;
    seed = dist(gen);
}

void GameSetupState::initialize()
{
    auto title = add_child<Label>(layer, Application::instance().font, "The Game", text_size * 3).lock();
    center(title, 20.f);
    menu_ptr = add_child<Menu>().lock();

    seed_l = menu_ptr->add_item(SelectableLabel([&](){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist;
        seed = dist(gen);
    }, layer, Application::instance().font, "Seed: " + std::to_string(seed), text_size));

    d_size_l = menu_ptr->add_item(SelectableLabel([&](){
        ++selected_d_size;
        if(selected_d_size >= dungeon_sizes.size())
        {
            selected_d_size = 0;
        }
    }, layer,
    Application::instance().font,
    "Dungeon size: " + std::to_string(dungeon_sizes[selected_d_size]), text_size));

    c_size_l = menu_ptr->add_item(SelectableLabel([&](){
        ++selected_c_size;
        if(selected_c_size >= chunk_sizes.size())
        {
            selected_c_size = 0;
        }
    }, layer,
    Application::instance().font,
    "Chunk_size: " + std::to_string(chunk_sizes[selected_c_size]), text_size));

    menu_ptr->add_item(SelectableLabel([&](){
        Application::instance().get_manager<GameStateManager>()->change_state<GameState>(
            seed, 
            sf::Vector2u{dungeon_sizes[selected_d_size], dungeon_sizes[selected_d_size]},
            chunk_sizes[selected_c_size]
        );
    }, layer, Application::instance().font, "Approve", text_size));

    menu_ptr->add_item(SelectableLabel([&](){
        Application::instance().get_manager<GameStateManager>()->change_state<MainMenuState>();
    }, layer, Application::instance().font, "Return", text_size));


    menu_ptr->setPosition(title->getPosition() +
                          sf::Vector2f{0.f, title->getGlobalBounds().size.y + 80.f});
    menu_ptr->set_alignment(Menu::Style::align_center);
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