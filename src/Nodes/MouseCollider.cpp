#include "Nodes/Application.hpp"
#include "Nodes/MouseCollider.hpp"
#include "Nodes/CollisionManager.hpp"
#include "Nodes/RenderManager.hpp"

void MouseCollider::on_collision(const std::weak_ptr<Collider> other)
{
    this->kill();
}

MouseCursor::MouseCursor(const std::string& view, const std::string& coll) : view_layer(view), collision_layer(coll){}

void MouseCursor::initialize()
{
    trigger = add_child<MouseTrigger>(sf::Vector2f{0, 0}, 0.f);

    auto ev_manager = Application::instance().get_manager<WindowEventManager>();
    auto coll_manager = Application::instance().get_manager<CollisionManager>();

    coll_manager->add_collider(collision_layer, trigger);

    ev_manager->get_event<sf::Event::MouseButtonPressed>()->subscribe(
        [&](const sf::Event::MouseButtonPressed& e)
        {
            if (e.button == sf::Mouse::Button::Left)
            {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(Application::instance().get_window());
                sf::Vector2f mouse_translated = Application::instance().get_window().mapPixelToCoords(mouse_pos);
                sf::Vector2f layer_pos = Application::instance().get_manager<RenderManager>()->
                            translate_to_layer(view_layer, mouse_translated);

                collider = add_child<MouseCollider>(layer_pos, 0.f);
                coll_manager->add_collider(collision_layer, collider);
            }
        });
}

void MouseTrigger::on_entered(const std::weak_ptr<Collider> other) {}
void MouseTrigger::on_exit(const std::weak_ptr<Collider> other) {}

void MouseCursor::update(float delta)
{
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(Application::instance().get_window());
    sf::Vector2f mouse_translated = Application::instance().get_window().mapPixelToCoords(mouse_pos);

    trigger.lock()->position = mouse_translated;

    if(auto ptr = collider.lock())
    {
        this->remove_child(ptr);
    }
}