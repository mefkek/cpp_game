#pragma once
#include "Nodes/Colliders.hpp"

class MouseCollider : public CircleCollider {
public:
    MouseCollider(const sf::Vector2f& position);
    void update(float deltaTime) override;
private:
    bool shouldBeRemoved = false;
};