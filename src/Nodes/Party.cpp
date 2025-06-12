#include "Nodes/Party.hpp"
#include <cmath>

void Party::display(sf::Vector2f position, sf::Vector2f scale, sf::Vector2f sp_scale)
{
    constexpr const float distance = 1.5f * 16.f;    //change after debug
    float step = (2 * M_PI) / children.size();

    float angle = 0;
    for(auto& node : children)
    {
        if(auto actor = std::dynamic_pointer_cast<Actor>(node))
        {
            float x = cos(angle) * distance * scale.x;
            float y = -sin(angle) * distance * scale.y;

            actor->getSprite()->setPosition({position.x + x, position.y + y});
            actor->getSprite()->setScale(sp_scale);
            angle += step;
        }
    }
}