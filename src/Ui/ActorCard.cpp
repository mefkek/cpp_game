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
    if(auto name_ptr = name.lock())
    {
        name_ptr->setPosition(getPosition());
        name_ptr->move({padding, padding});
        if(auto ico_ptr = portriat.lock())
        {
            ico_ptr->setPosition(name_ptr->getPosition() + 
                sf::Vector2f{0.f, name_ptr->getGlobalBounds().size.y + padding});
            if(auto hp_ptr = stats[0].lock())
            {
                hp_ptr->setPosition(ico_ptr->getPosition());
                hp_ptr->move({ico_ptr->getGlobalBounds().size.x + padding,
                        name_ptr->getGlobalBounds().size.y + padding});
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
    auto texture_size = Application::instance().get_manager<RenderManager>()->
            get_render_texture("Debug_ui").getSize();
    
    if(auto ptr = actor.lock())
    {
        name = add_child<Label>(layer, Application::instance().font, ptr->get_name(), 30);
        portriat = add_child<Icon>(
            layer,
            Application::instance().atlas.get_texture(),
            ptr->get_sprite()->getTextureRect()
        );
        portriat.lock()->setScale({10.f, 10.f});
        stats.push_back(add_child<Label>(
            layer,
            Application::instance().font, 
            std::to_string(ptr->get_stat("HP")), 30));
        stats[0].lock()->setFillColor(sf::Color::Red);
    }

    setSize({
            (texture_size.x * 0.7f) / 5.f,
            texture_size.y * 0.3f});
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