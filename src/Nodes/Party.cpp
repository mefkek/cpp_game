#include "Nodes/Party.hpp"
#include <cmath>

void Party::display(sf::Vector2f position, sf::Vector2f facing)
{
    constexpr const float distance = 1.5f * 16.f * 3.f;    //change after debug
    float step = (2 * M_PI) / children.size();

    float angle = 0;
    for(auto& node : children)
    {
        if(auto actor = std::dynamic_pointer_cast<Actor>(node))
        {
            float x = cos(angle) * distance;
            float y = -sin(angle) * distance;

            actor->getSprite()->setPosition({position.x + x, position.y + y});
            angle += step;
        }
    }
}