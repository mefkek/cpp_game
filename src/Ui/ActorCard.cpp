#include "Ui/ActorCard.hpp"
#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"

constexpr const char layer[] = "Debug_ui";
constexpr const float padding = 10.f;

ActorCard::ActorCard(std::weak_ptr<Actor> actor_ptr, sf::Vector2f position) : actor(actor_ptr)
{
    setPosition(position);
}

void ActorCard::set_actor(std::weak_ptr<Actor> actor_ptr)
{
    actor = actor_ptr;
}

void ActorCard::reposition()
{
    if(auto ico_ptr = portriat.lock())
    {
        ico_ptr->setPosition(getPosition() + sf::Vector2f{padding, padding});
        if(auto name_ptr = name.lock())
        {
            name_ptr->setPosition(ico_ptr->getPosition());
            name_ptr->move({ico_ptr->getGlobalBounds().size.x + padding, 0.f});
            if(auto hp_ptr = stats[0].lock())
            {
                hp_ptr->setPosition(name_ptr->getPosition());
                hp_ptr->move({0.f, name_ptr->getGlobalBounds().size.y + padding});
            }
        }
    }
}

void ActorCard::initialize()
{
    Application::instance().get_manager<RenderManager>()->add_drawable(
        layer,
        std::dynamic_pointer_cast<sf::Drawable>(shared_from_this())
    );

    setOutlineColor(sf::Color::Cyan);
    setOutlineThickness(-5.f);
    setFillColor(sf::Color::Transparent);
    auto layer_size = Application::instance().get_manager<RenderManager>()->
        get_render_sprite(layer).getGlobalBounds().size;
    
    if(auto ptr = actor.lock())
    {
        name = add_child<Label>(layer, Application::instance().font, ptr->get_name(), 30);
        portriat = add_child<Icon>(
            layer,
            Application::instance().atlas.get_texture(),
            ptr->getSprite()->getTextureRect()
        );
        portriat.lock()->setScale({10.f, 10.f});
        stats.push_back(add_child<Label>(
            layer,
            Application::instance().font, 
            std::to_string(ptr->get_stat("HP")), 30));
        stats[0].lock()->setFillColor(sf::Color::Red);
    }

    setSize({//portriat.lock()->getGlobalBounds().size.x + padding * 2 + name.lock()->getGlobalBounds().size.x,
            layer_size.x * 0.7f / 5.f,
            portriat.lock()->getGlobalBounds().size.y});
    reposition();
}

void ActorCard::update(float delta)
{
    reposition();

    if(auto ptr = actor.lock())
    {
        if(auto s_ptr = stats[0].lock())
        {
            s_ptr->setString(std::to_string(ptr->get_stat("HP")));
        }
    }
}