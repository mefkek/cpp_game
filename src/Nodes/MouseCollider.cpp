#include "Nodes/MouseCollider.hpp"

MouseCollider::MouseCollider(const sf::Vector2f& position)
    : CircleCollider(position, 0.0f) {
}

void MouseCollider::update(float deltaTime) {
    shouldBeRemoved = true;
}