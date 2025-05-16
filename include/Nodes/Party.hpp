#pragma once

#include "Nodes/Actor.hpp"
#include "Nodes/ContainerNode.hpp"

// A Party is a container node for Actor instances.
// Child management and update propagation are handled by ContainerNode.
class Party : public ContainerNode<Actor>
{
public:
    Party() = default;
};
