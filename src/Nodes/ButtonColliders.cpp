#include "Nodes/ButtonColliders.hpp"

ButtonCollider::ButtonCollider(sf::Vector2f size, sf::Vector2f pos) : RectangleCollider(pos, size) {}

void ButtonCollider::on_collision(const std::weak_ptr<Collider> other) {}

ButtonTriggerArea::ButtonTriggerArea(sf::Vector2f size, sf::Vector2f position) : TriggerArea(position, size) {}

void ButtonTriggerArea::on_entered(const std::weak_ptr<Collider> other) {}
void ButtonTriggerArea::on_exit(const std::weak_ptr<Collider> other) {};