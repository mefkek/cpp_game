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

void MainMenuState::initialize()
{
    auto title = add_child<Label>(layer, Application::instance().font, "The Game", text_size * 3).lock();
    center(title, 20.f);

    auto menu_ptr = add_child<Menu>().lock();
    menu_ptr->setPosition(title->getPosition() +
        sf::Vector2f{0.f, title->getGlobalBounds().size.y + 80.f});
    menu_ptr->set_alignment(Menu::Style::align_center);

    menu_ptr->add_item(SelectableLabel([](){
        Application::instance().get_manager<GameStateManager>()->change_state<GameSetupState>();
    }, layer, Application::instance().font, "Play", text_size));

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
    }, layer, Application::instance().font, "Load", text_size));

    menu_ptr->add_item(SelectableLabel([](){
        Application::instance().close();
    }, layer, Application::instance().font, "Quit", text_size));
}

void MainMenuState::update(float delta) {}