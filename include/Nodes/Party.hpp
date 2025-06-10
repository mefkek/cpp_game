#pragma once

#include <SFML/Graphics.hpp>
#include "Nodes/ContainerNode.hpp"
#include "Nodes/Actor.hpp"

// A Party is a container node for Actor instances.
// Child management and update propagation are handled by ContainerNode.
class Party : public ContainerNode<Actor>
{
public:
    void display(sf::Vector2f position, sf::Vector2f facing);
};
