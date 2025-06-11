#pragma once
#include "Nodes/CollisionManager.hpp"
#include "Nodes/TriggerArea.hpp"

class MouseCollider : public CircleCollider
{
private:
    int updated = false;
public:
    using CircleCollider::CircleCollider;
    void update(float delta) override;
    void on_collision(const std::weak_ptr<Collider> other) override;
};

class MouseTrigger : public TriggerArea
{
public:
    using TriggerArea::TriggerArea;
    void on_entered(const std::weak_ptr<Collider> other) override;
    void on_exit(const std::weak_ptr<Collider> other) override;
};

class MouseCursor : public Node
{
private:
    std::weak_ptr<MouseTrigger> trigger;
    std::weak_ptr<CircleCollider> collider;
    std::string view_layer;
    std::string collision_layer;
public:
    MouseCursor(const std::string& view, const std::string& coll);
    void update(float deltaTime) override;
    void initialize() override;
};