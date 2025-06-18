#pragma once
#include "Colliders.hpp"
#include "Node.hpp"
#include "Events.hpp"
#include <vector>
#include <memory>


class TriggerAreaColliderCircle : public CircleCollider
{
    private:
    std::vector<std::weak_ptr<Collider>> entered;
    std::vector<std::weak_ptr<Collider>> entered_frame;
    public:
    TriggerAreaColliderCircle(sf::Vector2f position, float size);
    void update(float delta) override;
    void on_collision(const std::weak_ptr<Collider> other) override;
};
class TriggerAreaColliderRect : public RectangleCollider
{
    private:
    std::vector<std::weak_ptr<Collider>> entered;
    std::vector<std::weak_ptr<Collider>> entered_frame;
    public:
    TriggerAreaColliderRect(sf::Vector2f position, sf::Vector2f size);
    void update(float delta) override;
    void on_collision(const std::weak_ptr<Collider> other) override;
};

class TriggerArea : public Node
{
    /*
        Specialized trigger area colliders
    */
    private:
    friend class CollisionManager;

    protected:
    bool didCollide = false;
    bool triggered = false;
    std::weak_ptr<Collider> last_collided_with;
    std::weak_ptr<CustomEvent<const std::weak_ptr<Collider>>> on_entered_ev;
    std::weak_ptr<CustomEvent<const std::weak_ptr<Collider>>> on_exit_ev;
    bool doCircle = false;


    public:
    sf::Vector2f position;
    sf::Vector2f size;
    std::weak_ptr<Collider> collider;
    TriggerArea(sf::Vector2f position, sf::Vector2f size);
    TriggerArea(sf::Vector2f position, float radius);
    void initialize() override;
    void update(float delta) override;
    virtual void on_entered(const std::weak_ptr<Collider> other) = 0;
    virtual void on_exit(const std::weak_ptr<Collider> other) = 0;
    std::weak_ptr<CustomEvent<const std::weak_ptr<Collider>>> get_on_entered_event();
    std::weak_ptr<CustomEvent<const std::weak_ptr<Collider>>> get_on_exit_event();
    virtual ~TriggerArea() = default;
};