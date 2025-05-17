#include "Nodes/Colliders.hpp"
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
    bool collides;

    //get rectangle bounds, we assume that the position is it's center
    float left = position.x - size.x / 2.f;
    float right = position.x + size.x / 2.f;
    float top = position.y - size.y / 2.f;
    float bottom = position.y + size.y / 2.f;

    //check if circle
    if(auto circle = std::dynamic_pointer_cast<CircleCollider>(other))
    {
        float closestX = std::clamp(circle->position.x, left, right);
        float closestY = std::clamp(circle->position.y, top, bottom);

        float dx = circle->position.x - closestX;
        float dy = circle->position.y - closestY;
        collides = (dx * dx + dy * dy) <= (circle->radius * circle->radius);
    }
    else
    {
        //get other rectangle bounds
        float other_left = other->position.x - other->size.x / 2.f;
        float other_right = other->position.x + other->size.x / 2.f;
        float other_top = other->position.y - other->size.y / 2.f;
        float other_bottom = other->position.y + other->size.y / 2.f;

        collides = !(right <= other_left || left >= other_right || bottom <= other_top || top >= other_bottom);
    }

    return collides;
}

CircleCollider::CircleCollider(sf::Vector2f center, float r)
                               : radius(r), Collider(center, {r*2, r*2}) {}

bool CircleCollider::collides_with(const std::shared_ptr<Collider> other) const
{
    bool collides;

    //check if circle
    if (auto circle = std::dynamic_pointer_cast<CircleCollider>(other))
    {
        float dx = position.x - circle->position.x;
        float dy = position.y - circle->position.y;
        float dist_sq = dx * dx + dy * dy;
        float radius_sum = radius + circle->radius;
        collides = (dist_sq <= radius_sum * radius_sum);
    }
    else
    {
        sf::Vector2f rect_pos = other->position;
        sf::Vector2f rect_size = other->size;
        float closestX = std::clamp(position.x, rect_pos.x - rect_size.x / 2.f, rect_pos.x + rect_size.x / 2.f);
        float closestY = std::clamp(position.y, rect_pos.y - rect_size.x / 2.f, rect_pos.y + rect_size.y / 2.f);

        float dx = position.x - closestX;
        float dy = position.y - closestY;
        collides = (dx * dx + dy * dy) <= (radius * radius);
    }

    return collides;
}

