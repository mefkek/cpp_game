#include "GameStateManager/GameState.hpp"
#include "GameStateManager/GameStateManager.hpp"
#include "Nodes/Application.hpp"

constexpr const char layer[] = "Debug_ui";

void MainMenuState::initialize()
{
    add_child<Label>(layer, Application::instance().font, "The Game", 60);
    auto menu_ptr = add_child<Menu>().lock();
    menu_ptr->add_item(SelectableLabel([](){
        Application::instance().get_manager<GameStateManager>()->change_state<GameState>();
    }, layer, Application::instance().font, "Play"));
    menu_ptr->add_item(SelectableLabel([](){
        //read save file
        Application::instance().get_manager<GameStateManager>()->change_state<GameState>();
    }, layer, Application::instance().font, "Load"));
    menu_ptr->add_item(SelectableLabel([](){
        Application::instance().close();
    }, layer, Application::instance().font, "Load"));
}

void MainMenuState::update(float delta) {}
