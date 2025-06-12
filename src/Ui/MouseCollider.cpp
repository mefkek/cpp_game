#include "Nodes/Application.hpp"
#include "../../include/Ui/MouseCollider.hpp"
#include "Nodes/CollisionManager.hpp"
#include "Nodes/RenderManager.hpp"

void MouseCollider::update(float delta)
{
    if(updated)
    {
        kill();
        return;
    }

    updated = true;
}

void MouseCollider::on_collision(const std::weak_ptr<Collider> other)
{
    this->kill();
}

MouseCursor::MouseCursor(const std::string& view, const std::string& coll) : view_layer(view), collision_layer(coll) {}

void MouseCursor::initialize()
{
    trigger = add_child<MouseTrigger>(sf::Vector2f{0, 0}, 0.f);
    Application::instance().get_manager<CollisionManager>()->add_collider("Debug_coll_trig", trigger);
    auto ev_manager = Application::instance().get_manager<WindowEventManager>();

    ev_manager->get_event<sf::Event::MouseButtonPressed>()->subscribe(
        [&](const sf::Event::MouseButtonPressed& e)
        {
            if (e.button == sf::Mouse::Button::Left)
            {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(Application::instance().get_window());
                sf::Vector2f mouse_translated = Application::instance().get_window().mapPixelToCoords(mouse_pos);

                collider = add_child<MouseCollider>(mouse_translated, 0.f);
                Application::instance().get_manager<CollisionManager>()->add_collider(collision_layer, collider);
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
}