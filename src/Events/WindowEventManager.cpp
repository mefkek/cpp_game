#include "Events/WindowEventManager.hpp"
#include "Nodes/Application.hpp"
#include "Utility/Logger.hpp"
#include <iostream>
#include <algorithm>

void WindowEventManager::update(float delta)
{
    sf::RenderWindow& window = Application::instance().get_window();

    while(const std::optional event = window.pollEvent())
    {
        for(auto c : children)
        {
            if(auto p = std::dynamic_pointer_cast<ITryRise>(c))
            {
                p->try_rise(event);
            }
        }
    }
}