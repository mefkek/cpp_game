#pragma once

#include "Nodes/Node.hpp"
#include "Nodes/Party.hpp"
#include "Nodes/Actor.hpp"
#include "Nodes/ActorBehaviour.hpp"
#include <memory>

// Manages creation and grouping of Actor instances.
class ActorManager : public Node
{
public:
    ActorManager();
    ~ActorManager() override = default;

    // Spawn a new Actor and assign it to the appropriate Party.
    std::shared_ptr<Actor> addActor(ActorRaceEnum race,
                                    std::shared_ptr<ActorBehaviour> behaviour);

    // No override of update(): Node::update(delta) will propagate through the tree.

private:
    std::shared_ptr<Party> playerParty_;
    std::shared_ptr<Party> enemyParty_;
};
