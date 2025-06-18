#include "GameStateManager/GameStateManager.hpp"

void GameStateManager::initialize()
{
    current = add_child<MainMenuState>();
}

void GameStateManager::update(float delta)
{
}