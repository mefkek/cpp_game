#pragma once
#include "Node.hpp"
#include "Events.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

class Collider : public Node
{
    private:
    std::weak_ptr<CustomEvent<const std::weak_ptr<Collider>>> on_collision_event;

    public:
    sf::Vector2f position;
    sf::Vector2f size;

    Collider(sf::Vector2f position, sf::Vector2f size);
    virtual ~Collider() = default;

    void update(float delta) override {}
    void initialize() override;


    virtual bool collides_with(const std::shared_ptr<Collider> other) const = 0;
    virtual void on_collision(const std::weak_ptr<Collider> other) = 0;
    std::weak_ptr<CustomEvent<const std::weak_ptr<Collider>>> get_on_collision_event() const;
};

class RectangleCollider : public Collider
{
    public:
    RectangleCollider(sf::Vector2f pos, sf::Vector2f size_);
    bool collides_with(const std::shared_ptr<Collider> other) const override;
};

class CircleCollider : public Collider
{
    public:
    float radius;

    CircleCollider(sf::Vector2f center, float r);

    bool collides_with(const std::shared_ptr<Collider> other) const override;
};

