#pragma once
#include <SFML/Graphics.hpp>
#include "Label.hpp"
#include "Icon.hpp"
#include "Nodes/Actor.hpp"
#include "Nodes/Node.hpp"

class ActorCard : public sf::RectangleShape, public Node
{
    private:
    std::weak_ptr<Actor> actor;
    std::weak_ptr<Label> name;
    std::weak_ptr<Icon> portriat;
    std::vector<std::weak_ptr<Label>> stats;
    void reposition();

    public:
    ActorCard(std::weak_ptr<Actor> actor_ptr, sf::Vector2f position = {0.f, 0.f});
    void initialize() override;
    void update(float delta) override;
    void set_actor(std::weak_ptr<Actor> actor_ptr);
};