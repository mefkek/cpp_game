#include "Nodes/Party.hpp"
#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "Ui/ActorCard.hpp"
#include <cmath>

void Party::display(sf::Vector2f position, sf::Vector2f scale, sf::Vector2f sp_scale, bool do_cards)
{
    constexpr const float distance = 1.5f * 16.f;    //change after debug
    float step = (2 * M_PI) / children.size();

    float angle = 0;
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        if(auto actor = std::dynamic_pointer_cast<Actor>(*it))
        {
            float x = cos(angle) * distance * scale.x;
            float y = -sin(angle) * distance * scale.y;

            actor->getSprite()->setPosition({position.x + x, position.y + y});
            actor->getSprite()->setScale(sp_scale);
            angle += step;
        }
    }
}

void Party::display_cards()
{
    auto window_size = Application::instance().get_window().getSize();
    int counter = 0;
    std::vector<std::weak_ptr<Actor>> to_ref;

    for(auto ch : children)
    {    
        if (std::shared_ptr<Actor> actor = std::dynamic_pointer_cast<Actor>(ch))
        {
            to_ref.push_back(actor);
        }
    }

    for(auto& ref: to_ref)
    {
        auto texture_size = Application::instance().get_manager<RenderManager>()->
            get_render_texture("Debug_ui").getSize();

        add_child<ActorCard>
        (
            ref,
            sf::Vector2f{((texture_size.x) * 0.7f) / 5.f * counter, texture_size.y * 0.7f}
        );
        ++counter;
    }
}