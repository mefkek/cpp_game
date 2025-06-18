#pragma once
#include "../Nodes/Colliders.hpp"
#include "../Nodes/TriggerArea.hpp"

class ButtonCollider : public RectangleCollider
{
    public:
    ButtonCollider(sf::Vector2f size, sf::Vector2f pos = {0, 0});

    void on_collision(const std::weak_ptr<Collider> other) override;
};

class ButtonTriggerArea : public TriggerArea
{
    public:
    ButtonTriggerArea(sf::Vector2f size, sf::Vector2f position = {0, 0});

    void on_entered(const std::weak_ptr<Collider> other) override;
    void on_exit(const std::weak_ptr<Collider> other) override;
};
