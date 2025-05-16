#include "Nodes/Colliders.hpp"
#include "Nodes/TriggerArea.hpp"
#include "Utility/Logger.hpp"
#include <algorithm>


Collider::Collider(sf::Vector2f position, sf::Vector2f size)
                   : position(position), size(size) {}

void Collider::initialize()
{
    on_collision_event = add_child<CustomEvent<const std::weak_ptr<Collider>>>();
    on_collision_event.lock()->subscribe(shared_from_this(), [this](const std::weak_ptr<Collider> other)
                                                             {
                                                                this->on_collision(other);
                                                             });
}

std::weak_ptr<CustomEvent<const std::weak_ptr<Collider>>> Collider::get_on_collision_event() const
{
    return on_collision_event;
}

RectangleCollider::RectangleCollider(sf::Vector2f pos, sf::Vector2f size_)
                                     : Collider(pos, size_) {}

bool RectangleCollider::collides_with(const std::shared_ptr<Collider> other) const
{
    bool collides =!(position.x + size.x < other->position.x ||
                     position.x > other->position.x + other->size.x ||
                     position.y + size.y < other->position.y ||
                     position.y > other->position.y + other->size.y);
    if(collides)
    {
        if(auto ev_ptr = this->get_on_collision_event().lock())
        {
            ev_ptr->rise(other);
        }
        else
        {
            Logger::log(Logger::MessageType::Warning, shared_from_this(), ": invalid collision event");
        }
    }

    return collides;
}

CircleCollider::CircleCollider(sf::Vector2f center, float r)
                               : radius(r), Collider(center, {r*2, r*2}) {}

bool CircleCollider::collides_with(const std::shared_ptr<Collider> other) const
{
    bool collides;
    if (auto circle = std::dynamic_pointer_cast<CircleCollider>(other)) 
    {
        float dx = position.x - circle->position.x;
        float dy = position.y - circle->position.y;
        float dist_sq = dx * dx + dy * dy;
        float radius_sum = radius + circle->radius;
        collides = dist_sq <= radius_sum * radius_sum;
    }
    else
    {
        sf::Vector2f rect_pos = other->position;
        sf::Vector2f rect_size = other->size;
        float closestX = std::clamp(position.x, rect_pos.x, rect_pos.x + rect_size.x);
        float closestY = std::clamp(position.y, rect_pos.y, rect_pos.y + rect_size.y);

        float dx = position.x - closestX;
        float dy = position.y - closestY;
        collides = (dx * dx + dy * dy) <= (radius * radius);
    }

    if(collides)
    {
        if(auto ev_ptr = get_on_collision_event().lock())
        {
            ev_ptr->rise(other);
        }
        else
        {
            Logger::log(Logger::MessageType::Warning, shared_from_this(), ": invalid collision event");
        }
    }

    return collides;
}

